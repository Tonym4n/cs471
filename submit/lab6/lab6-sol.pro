% f(1, 2, z) = f(_, _, X).
% head(a, tail(z, B), Y) = head(_, tail(X, _), _).
% cons(a, cons(b, cons(c, z))) = cons(_, cons(_, cons(_, X))).

% [1, 2, 3, 4] = [_|[_|Cddr]].
% [1, 2, 3, 4] = [_|[_|[Caddr|_]]].
% [[1, 2], 3, 4] = [[_|Cdar]|_].


quadratic_roots(A, B, C, Z) :-
	Z is (-B + sqrt((B ^ 2) - (4 * A * C))) / (2 * A);
	Z is (-B - sqrt((B ^ 2) - (4 * A * C))) / (2 * A).


quadratic_roots2(A, B, C, Z) :-
	Discr is (B ^ 2) - (4 * A * C),
	quadratic_roots(A, B, C, Discr, Z).

quadratic_roots(A, B, C, Discr, Z) :-
	Z is (-B + sqrt(Discr)) / (2 * A);
	Z is (-B - sqrt(Discr)) / (2 * A).


sum_lengths([], 0).
sum_lengths([X|Xs], L) :-
	sum_lengths(Xs, XsLen),
	length(X, XLen),
	L is XLen + XsLen.