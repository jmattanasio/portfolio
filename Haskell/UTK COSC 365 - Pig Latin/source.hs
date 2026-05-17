breakVowel :: String -> (String, String)
breakVowel str = break (`elem` "aeiouAEIOU") str

reorder :: (String, String) -> String
reorder (prevowel, vowelAndOn) = vowelAndOn ++ prevowel

piggify :: (String, String) -> String -> String
piggify (prevowel, vowelAndOn) reorderedStr
    | null prevowel = reorderedStr ++ "yay"
    | otherwise = reorderedStr ++ "ay"

processWord :: String -> String
processWord word = 
    let substrings = breakVowel word
        vowelLeading = reorder substrings
    in piggify substrings vowelLeading

main = do
    input <- getLine
    let allWords = words input
    let result = map processWord allWords
    putStrLn (unwords result)
    
    