import Text.Printf (printf)

taxRates :: [(Double, Double)]
-- Write tax rates here
-- The left value is the income floor
-- The right value is the tax rate for that income floor
taxRates = [(0, 0.20), (1000, 0.25), (5000, 0.2871), (10000, 0.3129), (15000, 0.3815)]

calculateTaxes :: Double -> [(Double,Double)] -> Double
calculateTaxes dollars [] = 0.0

calculateTaxes dollars [(floor, rate)] = 
    if dollars <= floor
        then 0.0
        else (dollars - floor) * rate

calculateTaxes dollars ((floor, rate):(nextFloor, nextRate):rest) =
    if dollars <= floor
        then 0.0
        else if dollars >= nextFloor
            then (nextFloor - floor) * rate
                + calculateTaxes dollars ((nextFloor, nextRate):rest)
            else (dollars - floor) * rate

-- Write calculateTaxes here
-- The first parameter is the dollar amount.
-- The second parameter is a list of tuples containing the tax rates.


main :: IO ()
main = do
    input <- getLine
    let income = read input :: Double
    let taxAmount = calculateTaxes income taxRates
    let taxPercentage = if income == 0 then 0 else (taxAmount / income) * 100    
    printf "$%.2f\n" $ taxAmount
    printf "%.0f%%\n" taxPercentage
