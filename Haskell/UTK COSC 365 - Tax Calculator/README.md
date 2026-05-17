Assignment
Write a Haskell program that calculates the income tax of a certain individual given their income. This income tax will be a progressive income tax, so certain levels of an individual’s income are taxed at different rates.

Income Rates List
The income tax rates are listed below:



Example
If I make $13,500, then the $3,500 above $10,000 is taxed at 31.29%, then I tax ($5,000...$10,000] at 28.71%, which is exactly $5,000. Then, I tax ($1,000...$5,000] 25%. Finally, the last $1,000 is taxed at 20%.

Therefore, the tax on $13,500 is:

0×0.2871)+($4000×0.25)+($1000×.20)=$3730.6(

Haskell
Use the following signature to create a list of tuples containing the income cutoff and rate.

taxRates :: [(Double, Double)]
Using this list, create a function called calculateTaxes that calculates the taxes owed given the income.

calculateTaxes :: Double -> [(Double,Double)] -> Double
Input
The first line from standard input is the income as a Double.

You do not need to error check the input.

After the input is given, calculate the taxes on that income.

Output
Output the taxes owed first precise to two decimal digits (you can use printf in Haskell to do this). Then, on the next line, output the final tax percentage that the user will need to owe (see examples below).

Examples
Example 1
13500
$3730.65
28%
Example 2
217200.00
$81339.30
37%
Example 3
100
$20.00
20%
Example 4
5000
$1200.00
24%
Example 5
3712.22
$878.06
24%