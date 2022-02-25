(define quadratic-roots 
    (lambda (a b c (func sqrt))
        (if (= a 0)
            'error
            {let 
                ([d (func (- (expt b 2) (* 4 a c)))]) 
                (list [/ (+ (- b) d) (* 2 a)] [/ (- (- b) d) (* 2 a)])})))

(define my-sqrt 
    (lambda (n (guess 1.0)) 
        (cond 
            [{> (abs (/ (- (* guess guess) n) n)) 0.0001} {my-sqrt n (/ (+ guess (/ n guess)) 2)}]
            [else guess])))

(define cmp-gt 
    (lambda (ls1 ls2) 
        (if (null? ls1)
            '()
            (cons (> (car ls1) (car ls2)) 
                (cmp-gt (cdr ls1) (cdr ls2))))))
                
(define ls-prod 
    (lambda (ls1 ls2) 
        (if (null? ls1)
            '()
            (cons (* (car ls1) (car ls2)) 
                (ls-prod (cdr ls1) (cdr ls2))))))
            
(define ls-f 
    (lambda (ls1 ls2 (func (lambda (a b) (+ a b)))) 
        (if (null? ls1)
            '()
            (cons (func (car ls1) (car ls2)) 
                (ls-f (cdr ls1) (cdr ls2) func)))))

(define greater-than 
    (lambda (ls (v 0)) 
        (if (null? ls)
            '()
            (cons (> (car ls) v) 
                (greater-than (cdr ls) v)))))