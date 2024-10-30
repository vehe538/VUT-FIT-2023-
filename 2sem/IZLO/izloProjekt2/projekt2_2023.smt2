; @author ~ Marek Čupr (xcuprm01)
; @date ~ 23. 4. 2023
; @subject ~ IZLO - project 2
; @body ~ 10/10 

(set-logic NIA)

(set-option :produce-models true)
(set-option :incremental true)

; Deklarace promennych pro vstupy
; ===============================

; Ceny
(declare-fun c1 () Int)
(declare-fun c2 () Int)
(declare-fun c3 () Int)
(declare-fun c4 () Int)
(declare-fun c5 () Int)

; Kaloricke hodnoty
(declare-fun k1 () Int)
(declare-fun k2 () Int)
(declare-fun k3 () Int)
(declare-fun k4 () Int)
(declare-fun k5 () Int)

; Maximalni pocty porci
(declare-fun m1 () Int)
(declare-fun m2 () Int)
(declare-fun m3 () Int)
(declare-fun m4 () Int)
(declare-fun m5 () Int)

; Maximalni cena obedu
(declare-fun max_cena () Int)

; Deklarace promennych pro reseni
(declare-fun n1 () Int)
(declare-fun n2 () Int)
(declare-fun n3 () Int)
(declare-fun n4 () Int)
(declare-fun n5 () Int)
(declare-fun best () Int)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;; START OF SOLUTION ;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ;;;   CONDITION 1 (price)   ;;;
; ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; ni ~ number of portions ordered for that particular food
; ci ~ price of 1 portion for that particular food

; Make sure the total price of the food ordered is less or equal to the max price of the lunch allowed 

(assert (<= (+ (* n1 c1) (* n2 c2) (* n3 c3) (* n4 c4) (* n5 c5)) max_cena))

; ~ ((n1 * c1) + (n2 * c2) + (n3 * c3) + (n4 * c4) + (n5 * c5)) <= max_cena
; ~ ordered_food <= max_cena

; ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ;;; CONDITION 2 (portions) ;;;
; ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; ni ~ number of portions ordered for that particular food
; mi ~ number of portions available for that particular food

; Make sure the amount of each ordered portion is >= 0 and less or egual to the amount of portions available (0 <= ni <= mi)

(assert(and (>= n1 0) (>= n2 0) (>= n3 0) (>= n4 0) (>= n5 0) (<= n1 m1) (<= n2 m2) (<= n3 m3) (<= n4 m4) (<= n5 m5) ))

; ~ (n1 >= 0) && (n2 >= 0) && (n3 >= 0) && (n4 >= 0) && (n5 >= 0) && (n1 <= m1) && (n2 <= m2) && (n3 <= m3) && (n4 <= m4) && (n5 <= m5) 

; ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ;;; CONDITION 3 (calories) ;;;
; ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; ni ~ number of portions ordered for that particular food
; ki ~ caloric value of 1 portion for that particular food

; Assign the optimal caloric value to the "best" variable 

(assert (= best (+ (* n1 k1) (* n2 k2) (* n3 k3) (* n4 k4) (* n5 k5))))
; ~ best = ((n1 * k1) + (n2 * k2) + (n3 * k3) + (n4 * k4) + (n5 * k5))

; Make sure we're getting the most optimal value of calories (if the optimal value is indeed the most optimal and can't be improved)

(assert (forall ((n1 Int) (n2 Int) (n3 Int) (n4 Int) (n5 Int))
          (=> (and (<= (+ (* n1 c1) (* n2 c2) (* n3 c3) (* n4 c4) (* n5 c5)) max_cena) ; correct price
          (>= n1 0) (>= n2 0) (>= n3 0) (>= n4 0) (>= n5 0) (<= n1 m1) (<= n2 m2) (<= n3 m3) (<= n4 m4) (<= n5 m5)) ; correct portions
          ; Implication between the 2 lines above and the line below
          (>= best (+ (* n1 k1) (* n2 k2) (* n3 k3) (* n4 k4) (* n5 k5))) ; most optimal caloric value
        )
    )
)

; ~ (((n1 * c1) + (n2 * c2) + (n3 * c3) (n4 * c4) (n5 * c5) <= max_cena) && 
;     (n1 >= 0) && (n2 >= 0) && (n3 >= 0) && (n4 >= 0) && (n5 >= 0) &&
;     (n1 <= m1) && (n2 <= m2) && (n3 <= m3) && (n4 <= m4) && (n5 <= m5)) =>
;     (best >= ((n1 * k1) + (n2 * k2) + (n3 * k3) + (n4 * k4) + (n5 * k5))))

; SAT when   --> T => T || F => T || F => F
; UNSAT when --> T => F

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;; END OF SOLUTION ;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Testovaci vstupy
; ================

(echo "Test 1.a - ocekavany vystup je sat, promenna best ma hodnotu 34")
(check-sat-assuming (
  (= c1 7) (= c2 3) (= c3 6) (= c4 10) (= c5 8)
  (= k1 5) (= k2 2) (= k3 4) (= k4 7)  (= k5 3)
  (= m1 3) (= m2 2) (= m3 4) (= m4 1)  (= m5 3)
  (= max_cena 50)
))
(get-value (best n1 n2 n3 n4 n5))

(echo "Test 1.b - neexistuje jine reseni nez 34, ocekavany vystup je unsat")
(check-sat-assuming (
  (= c1 7) (= c2 3) (= c3 6) (= c4 10) (= c5 8)
  (= k1 5) (= k2 2) (= k3 4) (= k4 7)  (= k5 3)
  (= m1 3) (= m2 2) (= m3 4) (= m4 1)  (= m5 3)
  (= max_cena 50)
  (not (= best 34))
))

; =========================================================

(echo "Test 2.a - ocekavany vystup je sat, promenna best ma hodnotu 0")
(check-sat-assuming (
  (= c1 7) (= c2 3) (= c3 6) (= c4 10) (= c5 8)
  (= k1 5) (= k2 2) (= k3 4) (= k4 7)  (= k5 3)
  (= m1 3) (= m2 2) (= m3 4) (= m4 1)  (= m5 3)
  (= max_cena 0)
))
(get-value (best n1 n2 n3 n4 n5))

(echo "Test 2.b - neexistuje jine reseni nez 0, ocekavany vystup je unsat")
(check-sat-assuming (
  (= c1 7) (= c2 3) (= c3 6) (= c4 10) (= c5 8)
  (= k1 5) (= k2 2) (= k3 4) (= k4 7)  (= k5 3)
  (= m1 3) (= m2 2) (= m3 4) (= m4 1)  (= m5 3)
  (= max_cena 0)
  (not (= best 0))
))
