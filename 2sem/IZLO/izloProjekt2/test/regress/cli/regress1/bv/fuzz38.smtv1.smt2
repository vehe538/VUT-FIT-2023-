; DISABLE-TESTER: lfsc
(set-option :incremental false)
(set-info :status unsat)
(set-logic QF_BV)
(declare-fun v0 () (_ BitVec 1))
(declare-fun v1 () (_ BitVec 1))
(declare-fun v2 () (_ BitVec 14))
(declare-fun v3 () (_ BitVec 4))
(declare-fun v4 () (_ BitVec 1))
(check-sat-assuming ( (let ((_let_0 (bvudiv ((_ sign_extend 9) (concat v3 v1)) v2))) (let ((_let_1 (ite (bvsgt (concat v3 v1) (concat v3 v1)) (_ bv1 1) (_ bv0 1)))) (let ((_let_2 ((_ sign_extend 13) v0))) (let ((_let_3 (bvcomp _let_2 v2))) (let ((_let_4 ((_ sign_extend 9) (ite (bvsgt ((_ zero_extend 4) (_ bv416 10)) _let_0) (_ bv1 1) (_ bv0 1))))) (let ((_let_5 (bvmul ((_ sign_extend 9) v1) (bvmul (_ bv416 10) ((_ zero_extend 9) (ite (bvsgt (bvshl (_ bv416 10) ((_ zero_extend 9) v1)) (bvshl (_ bv416 10) ((_ zero_extend 9) v1))) (_ bv1 1) (_ bv0 1))))))) (let ((_let_6 (ite (distinct _let_0 ((_ zero_extend 13) (ite (bvsgt ((_ zero_extend 4) (_ bv416 10)) _let_0) (_ bv1 1) (_ bv0 1)))) (_ bv1 1) (_ bv0 1)))) (let ((_let_7 (bvsrem _let_2 v2))) (let ((_let_8 (bvudiv ((_ sign_extend 4) (_ bv416 10)) _let_7))) (let ((_let_9 ((_ rotate_left 0) v1))) (let ((_let_10 (bvurem ((_ sign_extend 13) (ite (bvsgt ((_ zero_extend 13) v0) (bvnand ((_ zero_extend 13) (ite (bvsgt ((_ zero_extend 4) (_ bv416 10)) _let_0) (_ bv1 1) (_ bv0 1))) _let_0)) (_ bv1 1) (_ bv0 1))) v2))) (let ((_let_11 (bvshl (bvnand ((_ zero_extend 13) (ite (bvsgt ((_ zero_extend 4) (_ bv416 10)) _let_0) (_ bv1 1) (_ bv0 1))) _let_0) ((_ sign_extend 13) v1)))) (let ((_let_12 (bvsdiv ((_ zero_extend 4) _let_5) _let_10))) (let ((_let_13 (ite (bvsgt ((_ zero_extend 3) v0) v3) (_ bv1 1) (_ bv0 1)))) (let ((_let_14 ((_ zero_extend 1) _let_0))) (let ((_let_15 (bvnor _let_5 ((_ sign_extend 9) _let_6)))) (let ((_let_16 (bvsrem ((_ zero_extend 9) v4) _let_5))) (let ((_let_17 ((_ zero_extend 13) _let_9))) (let ((_let_18 ((_ sign_extend 13) _let_3))) (let ((_let_19 ((_ zero_extend 13) _let_6))) (let ((_let_20 (and (not (xor (and (= (distinct (bvshl (_ bv416 10) ((_ zero_extend 9) v1)) _let_16) (or (distinct ((_ sign_extend 9) (ite (bvsgt ((_ zero_extend 13) v0) (bvnand ((_ zero_extend 13) (ite (bvsgt ((_ zero_extend 4) (_ bv416 10)) _let_0) (_ bv1 1) (_ bv0 1))) _let_0)) (_ bv1 1) (_ bv0 1))) (_ bv416 10)) (=> (distinct ((_ zero_extend 4) _let_16) _let_7) (bvsge (bvudiv _let_4 (bvshl (_ bv416 10) ((_ zero_extend 9) v1))) (_ bv416 10))))) (bvugt (bvnand ((_ zero_extend 13) (ite (bvsgt ((_ zero_extend 4) (_ bv416 10)) _let_0) (_ bv1 1) (_ bv0 1))) _let_0) ((_ sign_extend 13) v1))) (= (distinct v0 _let_1) (bvslt (bvnand ((_ zero_extend 13) (ite (bvsgt ((_ zero_extend 4) (_ bv416 10)) _let_0) (_ bv1 1) (_ bv0 1))) _let_0) _let_19)))) (and (ite (= (or (and (= (ite (= (xor (or (= (_ bv416 10) ((_ sign_extend 9) v4)) (bvsgt _let_8 _let_11)) (= (bvsle v2 _let_18) (= _let_15 (bvudiv _let_4 (bvshl (_ bv416 10) ((_ zero_extend 9) v1)))))) (= (bvudiv _let_4 (bvshl (_ bv416 10) ((_ zero_extend 9) v1))) ((_ zero_extend 9) (ite (bvsgt ((_ zero_extend 13) v0) (bvnand ((_ zero_extend 13) (ite (bvsgt ((_ zero_extend 4) (_ bv416 10)) _let_0) (_ bv1 1) (_ bv0 1))) _let_0)) (_ bv1 1) (_ bv0 1))))) (= (xor (or (= (_ bv416 10) ((_ sign_extend 9) v4)) (bvsgt _let_8 _let_11)) (= (bvsle v2 _let_18) (= _let_15 (bvudiv _let_4 (bvshl (_ bv416 10) ((_ zero_extend 9) v1)))))) (= (bvudiv _let_4 (bvshl (_ bv416 10) ((_ zero_extend 9) v1))) ((_ zero_extend 9) (ite (bvsgt ((_ zero_extend 13) v0) (bvnand ((_ zero_extend 13) (ite (bvsgt ((_ zero_extend 4) (_ bv416 10)) _let_0) (_ bv1 1) (_ bv0 1))) _let_0)) (_ bv1 1) (_ bv0 1))))) (=> (=> (distinct (ite (bvsgt ((_ zero_extend 4) (_ bv416 10)) _let_0) (_ bv1 1) (_ bv0 1)) _let_9) (bvult ((_ sign_extend 9) (ite (= (bvudiv _let_4 (bvshl (_ bv416 10) ((_ zero_extend 9) v1))) ((_ sign_extend 6) (bvxnor ((_ sign_extend 3) (ite (bvsgt (bvshl (_ bv416 10) ((_ zero_extend 9) v1)) (bvshl (_ bv416 10) ((_ zero_extend 9) v1))) (_ bv1 1) (_ bv0 1))) v3))) (_ bv1 1) (_ bv0 1))) (bvshl (_ bv416 10) ((_ zero_extend 9) v1)))) (bvslt ((_ sign_extend 13) _let_1) _let_11))) (distinct _let_10 ((_ zero_extend 4) (_ bv416 10)))) (= (= _let_8 _let_0) (bvult v3 ((_ zero_extend 3) _let_13)))) (= (bvuge (bvnand ((_ zero_extend 13) (ite (bvsgt ((_ zero_extend 4) (_ bv416 10)) _let_0) (_ bv1 1) (_ bv0 1))) _let_0) ((_ zero_extend 13) (ite (bvsgt ((_ zero_extend 4) (_ bv416 10)) _let_0) (_ bv1 1) (_ bv0 1)))) (bvsge (bvshl (_ bv416 10) ((_ zero_extend 9) v1)) _let_16))) (= (ite (xor (xor (bvuge _let_0 ((_ zero_extend 4) (bvshl (_ bv416 10) ((_ zero_extend 9) v1)))) (bvsge v0 _let_13)) (ite (not (bvsge (bvnand ((_ zero_extend 13) (ite (bvsgt ((_ zero_extend 4) (_ bv416 10)) _let_0) (_ bv1 1) (_ bv0 1))) _let_0) ((_ zero_extend 4) _let_15))) (and (bvslt (_ bv416 10) ((_ sign_extend 6) v3)) (bvugt _let_10 _let_17)) (bvugt _let_7 _let_10))) (not (not (bvult ((_ zero_extend 13) v0) _let_10))) (=> (bvsgt ((_ sign_extend 13) _let_9) _let_8) (bvule _let_17 _let_0))) (xor (ite (= (_ bv416 10) _let_15) (or (= (ite (= (bvudiv _let_4 (bvshl (_ bv416 10) ((_ zero_extend 9) v1))) ((_ sign_extend 6) (bvxnor ((_ sign_extend 3) (ite (bvsgt (bvshl (_ bv416 10) ((_ zero_extend 9) v1)) (bvshl (_ bv416 10) ((_ zero_extend 9) v1))) (_ bv1 1) (_ bv0 1))) v3))) (_ bv1 1) (_ bv0 1)) (ite (= (bvudiv _let_4 (bvshl (_ bv416 10) ((_ zero_extend 9) v1))) ((_ sign_extend 6) (bvxnor ((_ sign_extend 3) (ite (bvsgt (bvshl (_ bv416 10) ((_ zero_extend 9) v1)) (bvshl (_ bv416 10) ((_ zero_extend 9) v1))) (_ bv1 1) (_ bv0 1))) v3))) (_ bv1 1) (_ bv0 1))) (bvuge ((_ zero_extend 5) (concat v3 v1)) _let_15)) (bvsgt _let_12 ((_ sign_extend 4) (bvudiv _let_4 (bvshl (_ bv416 10) ((_ zero_extend 9) v1)))))) (=> (xor (xor (bvuge _let_14 ((_ sign_extend 5) _let_16)) (bvsge ((_ zero_extend 9) v4) _let_5)) (distinct ((_ sign_extend 13) _let_13) _let_12)) (=> (bvugt (ite (= (bvudiv _let_4 (bvshl (_ bv416 10) ((_ zero_extend 9) v1))) ((_ sign_extend 6) (bvxnor ((_ sign_extend 3) (ite (bvsgt (bvshl (_ bv416 10) ((_ zero_extend 9) v1)) (bvshl (_ bv416 10) ((_ zero_extend 9) v1))) (_ bv1 1) (_ bv0 1))) v3))) (_ bv1 1) (_ bv0 1)) (ite (= (bvudiv _let_4 (bvshl (_ bv416 10) ((_ zero_extend 9) v1))) ((_ sign_extend 6) (bvxnor ((_ sign_extend 3) (ite (bvsgt (bvshl (_ bv416 10) ((_ zero_extend 9) v1)) (bvshl (_ bv416 10) ((_ zero_extend 9) v1))) (_ bv1 1) (_ bv0 1))) v3))) (_ bv1 1) (_ bv0 1))) (bvuge _let_7 ((_ sign_extend 13) (ite (bvsgt ((_ zero_extend 4) (_ bv416 10)) _let_0) (_ bv1 1) (_ bv0 1))))))))) (ite (= (distinct _let_11 ((_ sign_extend 13) _let_9)) (ite (bvult _let_14 ((_ sign_extend 5) (_ bv416 10))) (distinct _let_11 ((_ sign_extend 13) (ite (bvsgt (bvshl (_ bv416 10) ((_ zero_extend 9) v1)) (bvshl (_ bv416 10) ((_ zero_extend 9) v1))) (_ bv1 1) (_ bv0 1)))) (bvsle _let_3 v4))) (ite (ite (bvule ((_ sign_extend 5) (bvmul (_ bv416 10) ((_ zero_extend 9) (ite (bvsgt (bvshl (_ bv416 10) ((_ zero_extend 9) v1)) (bvshl (_ bv416 10) ((_ zero_extend 9) v1))) (_ bv1 1) (_ bv0 1))))) _let_14) (or (= _let_10 ((_ zero_extend 13) (ite (bvsgt ((_ zero_extend 4) (_ bv416 10)) _let_0) (_ bv1 1) (_ bv0 1)))) (bvuge _let_0 v2)) (bvule _let_6 (bvcomp (bvshl (_ bv416 10) ((_ zero_extend 9) v1)) _let_15))) (bvsge ((_ sign_extend 3) v4) v3) (bvuge _let_11 _let_12)) (or (= (bvudiv _let_4 (bvshl (_ bv416 10) ((_ zero_extend 9) v1))) ((_ sign_extend 9) (ite (bvsgt ((_ zero_extend 13) v0) (bvnand ((_ zero_extend 13) (ite (bvsgt ((_ zero_extend 4) (_ bv416 10)) _let_0) (_ bv1 1) (_ bv0 1))) _let_0)) (_ bv1 1) (_ bv0 1)))) (bvslt ((_ zero_extend 3) v1) (bvxnor ((_ sign_extend 3) (ite (bvsgt (bvshl (_ bv416 10) ((_ zero_extend 9) v1)) (bvshl (_ bv416 10) ((_ zero_extend 9) v1))) (_ bv1 1) (_ bv0 1))) v3)))) (ite (= (distinct _let_11 ((_ sign_extend 13) _let_9)) (ite (bvult _let_14 ((_ sign_extend 5) (_ bv416 10))) (distinct _let_11 ((_ sign_extend 13) (ite (bvsgt (bvshl (_ bv416 10) ((_ zero_extend 9) v1)) (bvshl (_ bv416 10) ((_ zero_extend 9) v1))) (_ bv1 1) (_ bv0 1)))) (bvsle _let_3 v4))) (ite (ite (bvule ((_ sign_extend 5) (bvmul (_ bv416 10) ((_ zero_extend 9) (ite (bvsgt (bvshl (_ bv416 10) ((_ zero_extend 9) v1)) (bvshl (_ bv416 10) ((_ zero_extend 9) v1))) (_ bv1 1) (_ bv0 1))))) _let_14) (or (= _let_10 ((_ zero_extend 13) (ite (bvsgt ((_ zero_extend 4) (_ bv416 10)) _let_0) (_ bv1 1) (_ bv0 1)))) (bvuge _let_0 v2)) (bvule _let_6 (bvcomp (bvshl (_ bv416 10) ((_ zero_extend 9) v1)) _let_15))) (bvsge ((_ sign_extend 3) v4) v3) (bvuge _let_11 _let_12)) (or (= (bvudiv _let_4 (bvshl (_ bv416 10) ((_ zero_extend 9) v1))) ((_ sign_extend 9) (ite (bvsgt ((_ zero_extend 13) v0) (bvnand ((_ zero_extend 13) (ite (bvsgt ((_ zero_extend 4) (_ bv416 10)) _let_0) (_ bv1 1) (_ bv0 1))) _let_0)) (_ bv1 1) (_ bv0 1)))) (bvslt ((_ zero_extend 3) v1) (bvxnor ((_ sign_extend 3) (ite (bvsgt (bvshl (_ bv416 10) ((_ zero_extend 9) v1)) (bvshl (_ bv416 10) ((_ zero_extend 9) v1))) (_ bv1 1) (_ bv0 1))) v3))))) (xor (distinct v3 ((_ zero_extend 3) (ite (bvsgt ((_ zero_extend 4) (_ bv416 10)) _let_0) (_ bv1 1) (_ bv0 1)))) (ite (ite (bvslt (concat v3 v1) ((_ zero_extend 4) _let_3)) (bvugt ((_ zero_extend 4) _let_13) (concat v3 v1)) (or (and (bvuge (_ bv416 10) ((_ zero_extend 9) _let_6)) (= (bvule (_ bv416 10) _let_4) (bvult _let_1 v1))) (xor (bvsgt v4 v4) (= (bvule ((_ sign_extend 5) _let_15) _let_14) (distinct v0 (ite (bvsgt (bvshl (_ bv416 10) ((_ zero_extend 9) v1)) (bvshl (_ bv416 10) ((_ zero_extend 9) v1))) (_ bv1 1) (_ bv0 1))))))) (ite (or (= _let_19 _let_8) (not (and (distinct (_ bv416 10) _let_16) (distinct ((_ zero_extend 13) (ite (bvsgt ((_ zero_extend 13) v0) (bvnand ((_ zero_extend 13) (ite (bvsgt ((_ zero_extend 4) (_ bv416 10)) _let_0) (_ bv1 1) (_ bv0 1))) _let_0)) (_ bv1 1) (_ bv0 1))) _let_0)))) (= (= _let_16 ((_ sign_extend 9) (ite (bvsgt (bvshl (_ bv416 10) ((_ zero_extend 9) v1)) (bvshl (_ bv416 10) ((_ zero_extend 9) v1))) (_ bv1 1) (_ bv0 1)))) (or (bvsge ((_ zero_extend 13) v4) _let_10) (= _let_14 ((_ zero_extend 14) _let_9)))) (or (distinct (bvnand ((_ zero_extend 13) (ite (bvsgt ((_ zero_extend 4) (_ bv416 10)) _let_0) (_ bv1 1) (_ bv0 1))) _let_0) _let_18) (bvult ((_ zero_extend 13) _let_1) _let_12))) (xor (not (= (bvult _let_0 _let_11) (= (concat v3 v1) ((_ zero_extend 1) v3)))) (or (bvslt _let_10 ((_ zero_extend 13) (ite (bvsgt ((_ zero_extend 4) (_ bv416 10)) _let_0) (_ bv1 1) (_ bv0 1)))) (distinct v2 ((_ zero_extend 13) (ite (bvsgt ((_ zero_extend 4) (_ bv416 10)) _let_0) (_ bv1 1) (_ bv0 1)))))))))))) (let ((_let_21 (bvnot (_ bv0 14)))) (and (and (and (and (and (and (and (and (= (bvule ((_ sign_extend 6) v3) _let_16) (ite _let_20 (not (= (bvult _let_8 ((_ zero_extend 13) v1)) (bvsle ((_ sign_extend 4) _let_16) _let_11))) _let_20)) (not (= (bvshl (_ bv416 10) ((_ zero_extend 9) v1)) (_ bv0 10)))) (not (= _let_5 (_ bv0 10)))) (not (= _let_5 (bvnot (_ bv0 10))))) (not (= _let_10 (_ bv0 14)))) (not (= _let_10 _let_21))) (not (= v2 (_ bv0 14)))) (not (= v2 _let_21))) (not (= _let_7 (_ bv0 14)))))))))))))))))))))))))) ))