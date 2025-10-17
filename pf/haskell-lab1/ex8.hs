not' :: Bool -> Bool
not' b = case b of
  True  -> False
  False -> True

isItTheAnswer :: String -> Bool
isItTheAnswer s = case s of
  "Love" -> True
  _      -> False

absInt :: Int -> Int
absInt n = case (n >= 0) of
  True -> n
  _    -> -n