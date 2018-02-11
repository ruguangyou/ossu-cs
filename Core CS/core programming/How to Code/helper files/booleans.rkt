;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-reader.ss" "lang")((modname booleans) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
(require 2htdp/image)

; booleans: true, false
(> 1 2)
(>= 2 2)
(= 3 3)

(string=? "foo" "bar")

(define I1 (rectangle 10 20 "outline" "red"))
(define I2 (rectangle 20 10 "outline" "blue"))
(> (image-width I1) (image-width I2))

; (if <question>
;     <expression if true>
;     <expression if false>)
(if (> (image-width I1) (image-height I1))
    (image-width I1)
    (image-height I2))

(if (> (image-width I1) (image-height I1))
    "10"    ; value doesn't need to add ()
    "20")

; (and <expression_1> <expression_2> ...)
; (or <expression_1> <expression_2> ...)
; (not <expression>)