fun1(add)
{
    return add + 1;
}

fun2(add)
{
    return add + 1;
}

main()
{
    var test, i;
    test = 0;

    for (i : 0 .. 20000000)
    {
        test = test + fun1(i % 7) + fun2(i % 13);
    }

    return 0;
}
