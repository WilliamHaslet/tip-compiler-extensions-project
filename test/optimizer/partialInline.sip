add(v, a)
{
    return v + a;
}

mult(v, m)
{
    return v * m;
}

addMult(i)
{
    var a1, a2;
    a1 = add(i % 3, 1);
    a2 = add(i % 7, 1);
    return mult(a1, mult(a2, 2));
}

main()
{
    var test, i;
    test = 0;

    for (i : 0 .. 30000000)
    {
        test = addMult(i);
    }

    return 0;
}
