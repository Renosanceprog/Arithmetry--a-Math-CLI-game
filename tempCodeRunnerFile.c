void getValidBossXYZ(int *x, char op1, int *y, char op2, int *z, int diffRange)
{
    if ((op2 == '*' || op2 == '/') && (op1 == '+' || op1 == '-'))
    {
        getValidXY(y,op2,z,diffRange);
        *x = rng(min_VALUE+(diffRange/2), max_VALUE+diffRange);
    }
    else if ((op1 == '*' || op1 == '/') && (op2 == '+' || op2 == '-'))
    {
        getValidXY(x,op1,y,diffRange);
        *z = rng(min_VALUE+(diffRange/2), max_VALUE+diffRange);
    }
    else if ((op1 == '*') && (op2 == '*'))
    {
        getValidXY(x,op1,y,diffRange);
        *z = rng(min_VALUE+(diffRange/2), (max_VALUE+diffRange)/4);
    }
    else if ((op1 == '/') && (op2 == '/'))
    {
        *z = rng(2, (max_VALUE + diffRange) / 4 + 2); 
        int temp_answer = rng(1, (max_VALUE + diffRange) / 4 + 2);
        int op1Eval = (*z) * temp_answer; 
        *y = rng(2, (max_VALUE + diffRange) / 4 + 2);
        *x = op1Eval * (*y);
    }
    else
    {
        getValidXY(x,op1,y,diffRange);
        *z = rng(min_VALUE+(diffRange/2), max_VALUE+diffRange);
    }
}