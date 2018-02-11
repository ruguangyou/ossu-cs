;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname image) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
(require 2htdp/image)

(circle 10 "solid" "red")
(rectangle 30 50 "outline" "blue")
(text "hello" 24 "green")

(above (circle 5 "solid" "red")
       (circle 10 "solid" "green")
       (circle 15 "solid" "blue"))

(beside (circle 5 "outline" "red")
        (circle 10 "outline" "green")
        (circle 15 "outline" "blue"))

(overlay (circle 5 "solid" "red")
         (circle 10 "solid" "green")
         (circle 15 "solid" "blue"))

(overlay (regular-polygon 10 5 "solid" "blue")
         (regular-polygon 20 8 "solid" "purple")
         (regular-polygon 30 12 "solid" "orange"))