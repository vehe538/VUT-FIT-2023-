; EXPECT: sat
(set-logic ALL)
(set-option :incremental false)
(set-option :finite-model-find true)
(set-option :fmf-bound true)
(set-option :produce-models true)
(declare-fun X () (Set Int))
(declare-fun Y () (Set Int))
(assert (forall ((x Int)) (=> (set.member x X) (> x 0))))
(check-sat-assuming ( (=> (and (= (set.card X) 5) (= Y (set.union X (set.singleton 9)))) (<= (set.card Y) 4)) ))