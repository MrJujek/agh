-- Funkcja logiczna xor' z użyciem case...of
xor' :: (Bool, Bool) -> Bool
xor' p = case p of
  (True, False)  -> True
  (False, True)  -> True
  _              -> False

{-
  xor' zwraca True tylko wtedy,
  gdy dokładnie jeden z argumentów jest True
-}

roots :: (Double, Double, Double) -> (Double, Double)
roots (a, b, c) = ( (-b - d) / e, (-b + d) / e )
    where { d = sqrt (b * b - 4 * a * c);
    e = 2 * a }

collatz :: Int -> Int
collatz n = 
    let divides d n = n `mod` d == 0
        isEven n = divides 2 n
    in if isEven n
        then n `div` 2
        else 3 * n + 1