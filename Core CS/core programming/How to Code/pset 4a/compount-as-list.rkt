;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-reader.ss" "lang")((modname compount-as-list) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;; Another way to construct a list: use compound data

(define-struct list-piece (first next))
;; ListOfNumber is one of:
;;  - false
;;  - (make-list-piece Number ListOfNumber)
;; interp. a list of numbers
(define LON1 false)
(define LON2 (make-list-piece 2 false))
(define LON3 (make-list-piece 1 (make-list-piece 3 false)))
#;
(define (fn-for-lon lon)
  (cond [(false? lon) (...)]
        [else
         (... (list-piece-first lon)
              (fn-for-lon (list-piece-next lon)))]))

;; Template rules used:
;;  - one of: 2 cases
;;  - atomic distinct: false
;;  - compound: (make-list-piece Number ListOfNumber)
;;  - self-reference: (list-piece-next lon) is ListOfNumber