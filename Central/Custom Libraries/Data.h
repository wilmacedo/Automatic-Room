struct Data
{
    int temperature = -1;
    int input = -1;

    bool miss()
    {
        return
            temperature == -1 ||
            input == -1
            ? true : false;
    }
};
