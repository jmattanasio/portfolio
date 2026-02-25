
class Statistics {
    private int mNumValues;
    private int mSumOfValues;
    private int mProductOfValues;
    private int mMaximumValue;
    private int mMinimumValue;

    public Statistics() {
        /*This constructor initializes everything at zero besides the 
         * product attribute due to the multiplicative property, if it 
         * were zero, it would always equal zero.
         */
        mNumValues = 0;
        mSumOfValues = 0;
        mProductOfValues = 1;
        mMaximumValue = 0;
        mMinimumValue = 0;

    }
    
    public int getSum() {
        return mSumOfValues;
    }

    public int getNumValues() {
        return mNumValues;
    }

    public int getProduct() {
        return mProductOfValues;
    }

    public int getMin() {
        return mMinimumValue;
    }

    public int getMax() {
        return mMaximumValue;
    }
    public void addValue(int val) {
        /*This block of code adds the value by incrementing the 
        number of values member, adding the value to the sum, and 
        multiplying the value to the product. */
        mNumValues++;
        mSumOfValues += val;
        mProductOfValues *= val;
    
        /*This if statement ensures that if the user only
         * enter one integer that the value they enter is both the 
         * maximum and the minimum, if its alone. Otherwise we compare
         * each time an integer is entered in via the .addValue(val).
         */
        if (mNumValues == 1){
            mMaximumValue = val;
            mMinimumValue = val;
        }
        else {
            if (val > mMaximumValue){
                mMaximumValue = val;
            }
            if (val < mMinimumValue){
                mMinimumValue = val;
            }
        }
        
    }

    public double getAverage() {
        /*Returns the average and if there are no numbers in the "list" then 
         * the average is zero, rightfully so.
         */
        if (mNumValues > 0){
        return (double)mSumOfValues/mNumValues;
        }
        else {
            return 0.0;
        }
    }
}