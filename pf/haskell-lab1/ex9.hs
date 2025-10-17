unitVec2D :: (Double, Double) -> (Double, Double)
unitVec2D (x, y) = (x / len, y / len)
  where
    len = sqrt (x^2 + y^2)