;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-reader.ss" "lang")((modname struct) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))

;; struct definetion: (define-struct <struct-name> (<field-name> ...))
(define-struct pos (x y))

;; constructor: make-<struct-name>
(define P1 (make-pos 3 6))
(define P2 (make-pos 2 8))

;; selectors: <struct-name>-<field-name>
(pos-x P1)  ;3
(pos-y P2)  ;8

;; predicate: <struct-name>?
(pos? P1)  ;true
(pos? "hello")  ;false
