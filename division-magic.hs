import Data.Bits
import System.Environment
import Text.Printf

signedMin :: Word
signedMax :: Word
w :: Word

signedMin = 0x80000000
signedMax = 0x7fffffff
w = fromIntegral $ finiteBitSize (0 :: Word)

getNc :: Word -> Int -> Word
getNc d l = ones - (mod (ones + 1 - d) d :: Word)
    where ones = shiftR (negate 0 :: Word) l
    
guardClause :: Word -> Word -> Word -> Word -> Bool
guardClause p q1 r1 delta = (p < (w * 2) && (q1 < delta || (q1 == delta && r1 == 0)))

innerUMagic :: Word -> Word -> Word -> Word -> Word -> Word -> Word -> Int -> (Word, Word)
innerUMagic p d nc q1 r1 q2 r2 l = do
    let (q1t, r1t) = if r1 >= (nc - r1)
                     then ((shiftL q1 1) .|. 1, (shiftL r1 1) - nc)
                     else (shiftL q1 1, shiftL r1 1)
        (q2t, r2t) = if (r2 + 1) >= (d - r2)
                     then ((shiftL q2 1) .|. 1, ((shiftL r2 1) .|. 1) - d)
                     else (shiftL q2 1, (shiftL r2 1) .|. 1)
        pn = p + 1
    if guardClause pn q1t r1t (d - 1 - r2t)
       then innerUMagic pn d nc q1t r1t q2t r2t l
       else (q2t + 1, pn - w - 32) -- why - 32 ? idk man

getUMagic :: Word -> Int -> (Word, Word) -- Magic and Shift --
getUMagic d l = do
    let nc = getNc d l
        p = w - 1
        (q1, r1) = (divMod signedMin nc)
        (q2, r2) = (divMod signedMax d)
    innerUMagic p d nc q1 r1 q2 r2 l

main :: IO ()
main = do
    args <- getArgs
    if null args
       then putStrLn "Provide a constant divisor to calculate a magic reciprocal for"
       else do
           let divisor = read (head args) :: Word
               (m, s) = getUMagic divisor 0
           printf "%llx: %llx | %d" divisor m s
