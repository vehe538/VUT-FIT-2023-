; COMMAND-LINE: --dump-instantiations --incremental --print-inst-full
; SCRUBBER: sed -e 's/skv_.* )$/skv_TERM )/'
; EXPECT: unsat
; EXPECT: (skolem (forall ((x Int)) (or (P x) (Q x)))
; EXPECT:   ( skv_TERM )
; EXPECT: )
; EXPECT: (instantiations (forall ((x Int)) (P x))
; EXPECT:   ( skv_TERM )
; EXPECT: )
; EXPECT: unsat
; EXPECT: (skolem (forall ((x Int)) (or (P x) (R x)))
; EXPECT:   ( skv_TERM )
; EXPECT: )
; EXPECT: (instantiations (forall ((x Int)) (P x))
; EXPECT:   ( skv_TERM )
; EXPECT: )
; disable proofs since it impacts what is relevant (e.g. the skolem lemmas)
; DISABLE-TESTER: proof
(set-logic UFLIA)
(declare-fun P (Int) Bool)
(declare-fun Q (Int) Bool)
(assert (forall ((x Int)) (P x)))
(push 1)
(assert (exists ((x Int)) (and (not (P x)) (not (Q x)))))
(check-sat)
(pop 1)
(declare-fun R (Int) Bool)
(assert (exists ((x Int)) (and (not (P x)) (not (R x)))))
(check-sat)
