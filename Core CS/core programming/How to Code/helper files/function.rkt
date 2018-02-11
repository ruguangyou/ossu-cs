;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-reader.ss" "lang")((modname function) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
(require 2htdp/image)

; (define (<function name> <parameter(s)>)
;   (<function body / expression>))
(define (bulb c)
  (circle 40 "solid" c))

; call function
; (<name-of-defined-function> <expression> ... )
(bulb (string-append "re" "d"))

(beside (bulb "red") (bulb "yellow") (bulb "green"))
