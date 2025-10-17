sqr :: Double -> Double
sqr x = x * x

vec2DLen :: (Double, Double) -> Double
vec2DLen (x,y) = sqrt (sqr x + sqr y)

vec3DLen :: (Double, Double, Double) -> Double
vec3DLen (x,y,z) = sqrt (sqr x + sqr y + sqr z)

swap :: (a, b) -> (b, a)
swap (x,y) = (y,x)

threeEqual :: (Int, Int, Int) -> Bool
threeEqual (x,y,z) = (x == y) && (y == z)