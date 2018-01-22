;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-abbr-reader.ss" "lang")((modname space-invaders) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
(require 2htdp/universe)
(require 2htdp/image)

;; Space Invaders


;; Constants:

(define WIDTH  300)
(define HEIGHT 500)

(define INVADER-X-SPEED 1.5)  ;speeds (not velocities) in pixels per tick
(define INVADER-Y-SPEED 1.5)
(define TANK-SPEED 2)
(define MISSILE-SPEED 10)

(define HIT-RANGE 10)

(define INVADE-RATE 100)

(define BACKGROUND (empty-scene WIDTH HEIGHT))

(define INVADER
  (overlay/xy (ellipse 10 15 "outline" "blue")              ;cockpit cover
              -5 6
              (ellipse 20 10 "solid"   "blue")))            ;saucer

(define TANK
  (overlay/xy (overlay (ellipse 28 8 "solid" "black")       ;tread center
                       (ellipse 30 10 "solid" "green"))     ;tread outline
              5 -14
              (above (rectangle 5 10 "solid" "black")       ;gun
                     (rectangle 20 10 "solid" "black"))))   ;main body

(define TANK-HEIGHT/2 (/ (image-height TANK) 2))
(define TANK-WIDTH/2  (/ (image-width  TANK) 2))

(define MISSILE (ellipse 5 15 "solid" "red"))



;; Data Definitions:

(define-struct game (invaders missiles tank))
;; Game is (make-game  (listof Invader) (listof Missile) Tank)
;; interp. the current state of a space invaders game
;;         with the current invaders, missiles and tank position

;; Game constants defined below Missile data definition

#;
(define (fn-for-game s)
  (... (fn-for-loinvader (game-invaders s))
       (fn-for-lom (game-missiles s))
       (fn-for-tank (game-tank s))))



(define-struct tank (x dir))
;; Tank is (make-tank Number Integer[-1, 1])
;; interp. the tank location is x, HEIGHT - TANK-HEIGHT/2 in screen coordinates
;;         the tank moves TANK-SPEED pixels per clock tick left if dir -1, right if dir 1

(define T0 (make-tank (/ WIDTH 2) 1))   ;center going right
(define T1 (make-tank 50 1))            ;going right
(define T2 (make-tank 50 -1))           ;going left

#;
(define (fn-for-tank t)
  (... (tank-x t) (tank-dir t)))



(define-struct invader (x y dx))
;; Invader is (make-invader Number Number Number)
;; interp. the invader is at (x, y) in screen coordinates
;;         the invader along x by dx pixels per clock tick

(define I1 (make-invader 150 100 12))           ;not landed, moving right
(define I2 (make-invader 150 HEIGHT -10))       ;exactly landed, moving left
(define I3 (make-invader 150 (+ HEIGHT 10) 10)) ;> landed, moving right


#;
(define (fn-for-invader invader)
  (... (invader-x invader) (invader-y invader) (invader-dx invader)))


(define-struct missile (x y))
;; Missile is (make-missile Number Number)
;; interp. the missile's location is x y in screen coordinates

(define M1 (make-missile 150 300))                       ;not hit U1
(define M2 (make-missile (invader-x I1) (+ (invader-y I1) 10)))  ;exactly hit U1
(define M3 (make-missile (invader-x I1) (+ (invader-y I1)  5)))  ;> hit U1

#;
(define (fn-for-missile m)
  (... (missile-x m) (missile-y m)))



(define G0 (make-game empty empty T0))
(define G1 (make-game empty empty T1))
(define G2 (make-game (list I1) (list M1) T1))
(define G3 (make-game (list I1 I2) (list M1 M2) T1))



;; Functions:

;; Game -> Game
;; start space invader game by evaluating (main (make-game empty empty T0))
(define (main g)
  (big-bang g
    (on-key  handle-key)       ; Game KeyEvent -> Game
    (on-tick next)             ; Game -> Game
    (to-draw render)           ; Game -> Image
    (stop-when last-world?)))  ; Game -> Boolean

;; Game KeyEvent -> Game
;; if ke is "left" set dir -1, if ke is "right" set dir 1, otherwise don't change dir
;; if ke is " ", produce missile
(check-expect (handle-key (make-game empty empty (make-tank 50 1)) "up")
              (make-game empty empty (make-tank 50 1)))
(check-expect (handle-key (make-game empty empty (make-tank 50 1)) "left")
              (make-game empty empty (make-tank 50 -1)))
(check-expect (handle-key (make-game empty empty (make-tank 50 -1)) "right")
              (make-game empty empty (make-tank 50 1)))
(check-expect (handle-key (make-game empty (list M1) (make-tank 50 -1)) " ")
              (make-game empty (list (make-missile 50 (- HEIGHT TANK-HEIGHT/2)) M1) (make-tank 50 -1)))
;(define (handle-key g ke) g)  ;stub
(define (handle-key g ke)
  (cond [(key=? ke "left")
         (make-game (game-invaders g)
                    (game-missiles g)
                    (make-tank (tank-x (game-tank g)) -1))]
        [(key=? ke "right")
         (make-game (game-invaders g)
                    (game-missiles g)
                    (make-tank (tank-x (game-tank g)) 1))]
        [(key=? ke " ")
         (make-game (game-invaders g)
                    (cons (make-missile (tank-x (game-tank g))
                                        (- HEIGHT TANK-HEIGHT/2))
                          (game-missiles g))
                    (game-tank g))]
        [else g]))


;; Game -> Game
;; produce next game(invaders missiles tank)
(define (next g)  
  (make-game (next-invaders (game-invaders g) (game-missiles g))
             (next-missiles (game-invaders g) (game-missiles g))
             (next-tank (game-tank g))))

;; ListOfInvader -> ListOfInvader
;; produce list of invaders on tick
(define (next-invaders loi lom)
  (move-invaders (if (> (random (+ 3 INVADE-RATE)) INVADE-RATE)
                     (new-invaders (i-m loi lom))
                     (i-m loi lom))))

;; ListOfInvader ListOfMissile -> ListOfInvader
;; revome invaders that are hitted by missiles
(define (i-m loi lom)
  (cond [(empty? loi) empty]
        [else
         (if (i-hitted? (first loi) lom)
             (i-m (rest loi) lom)
             (cons (first loi) (i-m (rest loi) lom)))]))

;; Invader ListOfMissile -> Boolean
;; produce true if the invader is hitted
(define (i-hitted? i lom)
  (cond [(empty? lom) false]
        [else
         (or (and (>= (- (invader-x i) (missile-x (first lom))) (- HIT-RANGE))
                  (<= (- (invader-x i) (missile-x (first lom))) HIT-RANGE)
                  (>= (- (invader-y i) (missile-y (first lom))) (- HIT-RANGE))
                  (<= (- (invader-y i) (missile-y (first lom))) HIT-RANGE))
             (i-hitted? i (rest lom)))]))


;; ListOfInvader -> ListOfInvader
;; produce new invaders
(define (new-invaders loi)
  (cons (make-invader (random WIDTH)
                      0
                      (if (zero? (random 2)) INVADER-X-SPEED (- INVADER-X-SPEED)))
        loi))

;; ListOfInvader -> ListOfInvader
;; change the position of invaders
(define (move-invaders loi)
  (cond [(empty? loi) empty]
        [else
         (cons (move-invader (first loi))
               (move-invaders (rest loi)))]))

;; Invader -> Invader
;; move the invader with the given INVADER-X-SPEED and INVADER-Y-SPEED
(check-expect (move-invader (make-invader 50 50 1.5)) (make-invader 51.5 51.5 1.5))
(check-expect (move-invader (make-invader 0 50 -1.5)) (make-invader  1.5 51.5 1.5))
(check-expect (move-invader (make-invader WIDTH 50 1.5)) (make-invader (- WIDTH 1.5) 51.5 -1.5))
;(define (move-invader i) i)  ;stub
(define (move-invader i)
  (cond [(<= (invader-x i) 0)
         (make-invader (+ 0 INVADER-X-SPEED)
                       (+ (invader-y i) INVADER-Y-SPEED)
                       INVADER-X-SPEED)]
        [(>= (invader-x i) WIDTH)
         (make-invader (- WIDTH INVADER-X-SPEED)
                       (+ (invader-y i) INVADER-Y-SPEED)
                       (- INVADER-X-SPEED))]
        [else
         (make-invader (+ (invader-x i) (invader-dx i))
                       (+ (invader-y i) INVADER-Y-SPEED)
                       (invader-dx i))]))


;; ListOfMissile -> ListOfMissile
;; remove hitted missiles, and produce list of missiles on tick
(define (next-missiles loi lom)
  (cond [(empty? lom) empty]
        [else
         (if (m-hitted? (first lom) loi)
             (next-missiles loi (rest lom))
             (cons (move-missile (first lom))
               (next-missiles loi (rest lom))))]))

;; Missile ListOfInvader -> Boolean
;; produce true if the invader is hitted
(define (m-hitted? m loi)
  (cond [(empty? loi) false]
        [else
         (or (and (>= (- (missile-x m) (invader-x (first loi))) (- HIT-RANGE))
                  (<= (- (missile-x m) (invader-x (first loi))) HIT-RANGE)
                  (>= (- (missile-y m) (invader-y (first loi))) (- HIT-RANGE))
                  (<= (- (missile-y m) (invader-y (first loi))) HIT-RANGE))
             (m-hitted? m (rest loi)))]))

;; Missile -> Missile
;; increase the y coordinate of missile with given MISSILE-SPEED
(define (move-missile m)
  (make-missile (missile-x m) (- (missile-y m) MISSILE-SPEED)))


;; Tank -> Tank
;; move the tank with dir and TANK-SPEED
(define (next-tank t)
  (if (> (tank-dir t) 0)
      (if (> (tank-x t) (- WIDTH TANK-WIDTH/2))
          (make-tank (- WIDTH TANK-WIDTH/2) -1)
          (make-tank (+ (tank-x t) TANK-SPEED) 1))
      (if (< (tank-x t) TANK-WIDTH/2)
          (make-tank TANK-WIDTH/2 1)
          (make-tank (- (tank-x t) TANK-SPEED) -1))))


;; Game -> Image
;; render the game onto the background

;(define (render g) BACKGROUND)  ;stub
(define (render g)
  (overlay (render-invaders (game-invaders g))
           (render-missiles (game-missiles g))
           (render-tank (game-tank g))))

;; ListOfInvader -> Image
;; render list of invaders onto the background
(define (render-invaders loi)
  (cond [(empty? loi) (rectangle WIDTH HEIGHT "outline" "black")]
        [else
         (place-image INVADER
                      (invader-x (first loi))
                      (invader-y (first loi))
                      (render-invaders (rest loi)))]))

;; ListOfMissile -> Image
;; render list of missiles onto the background
(define (render-missiles lom)
  (cond [(empty? lom) (rectangle WIDTH HEIGHT "outline" "black")]
        [else
         (place-image MISSILE
                      (missile-x (first lom))
                      (missile-y (first lom))
                      (render-missiles (rest lom)))]))

;; Tank -> Image
;; render the tank
(define (render-tank t)
  (place-image TANK (tank-x t) (- HEIGHT TANK-HEIGHT/2) BACKGROUND))


;; Game -> Boolean
;; stop the world if true
(define (last-world? g)
  (landed? (game-invaders g)))

;; ListOfInvaders -> Boolean
;; if any invader landed, produce true
(define (landed? loi)
  (cond [(empty? loi) false]
        [else
         (or (>= (invader-y (first loi)) HEIGHT)
             (landed? (rest loi)))]))