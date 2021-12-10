fun2(test)
{
    var i;

    for (i : 0 .. 100)
    {
        test = test + (i % 3) + 1;
    }

    return test;
}

fun1(test)
{
    var i;

    for (i : 0 .. 100)
    {
        test = test * (i % 2) + 1;
    }

    return test;
}

main()
{
    var test, i, o;
    test = 0;

    for (i : 0 .. 1000000)
    {
        o = fun2(test);
        test = fun1(o);
    }

    return 0;
}
