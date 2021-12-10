fun1()
{
    var i, j, test, test2, test3;

    test = 0;
    test2 = 0;
    test3 = 0;

    for (i : 0 .. 100)
    {
        if (i % 2 == 0)
        {
            test++;
        }
        else 
        {
            test2++;
        }
    }

    for (j : 0 .. 100)
    {
        if (i % 2 == 0)
        {
            test2++;
        }
        else 
        {
            test3++;
        }
    }

    return 0;
}

main()
{
    var test, i;
    test = 0;

    for (i : 0 .. 1000000)
    {
        test = test + fun1();
    }

    return 0;
}
