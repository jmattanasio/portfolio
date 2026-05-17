import System.IO (hFlush, stdout)
import Text.Printf (printf)


getFactors :: Int -> Int -> [Int]
getFactors x iter
    | iter > x = []
    | x `mod` iter == 0 = iter : getFactors x (iter + 1)
    | otherwise = getFactors x (iter + 1)


getResults :: Int -> Int -> Int -> IO [Int]
getResults st fin curr
    | curr > fin = return []  -- stopping condition
    | otherwise = do
        let currStr = show curr
        printf "%5s: " currStr
        let currFactors = getFactors curr 1
        putStrLn $ unwords [printf "%d" x | x <- currFactors]
        getResults st fin (curr + 1)

main :: IO ()
main = do
    startStr <- getLine
    finishStr <- getLine
    let start = read startStr :: Int
    let finish = read finishStr :: Int
    let current = start
    getResults start finish current
    return()
    