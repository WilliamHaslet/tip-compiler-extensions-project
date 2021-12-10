testFun(arg1, arg2, arg3, arg4, arg5, arg6, arg7)
{
    return arg1 + 1;
}

testFun2(arg1, arg2, arg3, arg4, arg5, arg6, arg7)
{
    return arg1 + 1;
}

main()
{
    var test, i;
    test = 0;

    for (i : 0 .. 30000000)
    {
        test = test + testFun(testFun2(i % 2, i % 10, 4, 2, i, i, 40954), i % 10, 4, 2, i, i, 40954);
    }

    return 0;
}
