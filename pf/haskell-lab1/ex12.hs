f1 x = x
-- typ: ?

f2 x = True
-- typ: ?

f3 (x, y) = x + y
-- typ: ?

f4 (x, y) = x / y
-- typ: ?

f5 (x, y) = x == y
-- typ: ?

f6 (x, y) = if x > y then x else y
-- typ: ?

f7 xs = head xs
-- typ: ?

f8 xs = length xs
-- typ: ?

f9 (x:xs) = xs
-- typ: ?

f10 (x, _) = x
-- typ: ?

f11 f x = f (f x)
-- typ: ?

f12 f g x = f (g x)
-- typ: ?

f13 x = [x]
-- typ: ?

f14 x = (x, x)
-- typ: ?

f15 (x, y, z) = z
-- typ: ?

f16 (x, y) = if x >= y then x else y
-- typ: ?