; Atoms
(define nil `())
(define true 1)
(define false 0)

; flip the parameter of a function
(define (flip f a b) (f b a))
(define (ghost . xs) (eval xs))

; Apply function to list items
(define (map f l) (
    if (== l nil)
        (nil)
        (append (list (f (car l))) (map f (cdr l)))
))

; Apply filter to list items
(define (filter f l) (
    if (== l nil)
        (nil)
        (append (if (f (car l)) (list (car l)) (nil)) (filter f (cdr l)))
))

; Fold left
(define (foldl f z l) (
    if (== l nil)
        (z)
        (foldl f (f z (car l)) (cdr l))
))
