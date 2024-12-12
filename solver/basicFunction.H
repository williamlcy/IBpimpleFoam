scalar Delta(scalar R)
{
    scalar COF;

    if(0.5 <= R && R <= 1.5)
    {
        COF = 1.0/6*(5.0 - 3.0*R - Foam::sqrt(-3.0*(1.0-R)*(1.0-R)+1.0));
    }
    else if(R <= 0.5)
    {
        COF = 1.0/3*(1 + Foam::sqrt(-3.0*R*R+1.0));
    }
    else
    {
        COF = 0;
    }
    return COF;
}

scalar Delta1(scalar R)
{
    scalar COF;

    if(R <= 2)
    {
        COF = 0.25 * (1 + Foam::cos(Foam::constant::mathematical::pi * R / 2));
    }
    else
    {
        COF = 0;
    }
    return COF;
}