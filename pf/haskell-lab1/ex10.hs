unitVec2D :: (Double, Double) -> (Double, Double)
unitVec2D (x, y) =
  let len = sqrt (x^2 + y^2)
  in (x / len, y / len)