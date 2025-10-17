sgn :: Int -> Int
sgn n | n > 0     = 1
      | n == 0    = 0
      | otherwise = -1


min3Int :: (Int, Int, Int) -> Int
min3Int (x, y, z)
  | x <= y && x <= z = x
  | y <= z           = y
  | otherwise        = z