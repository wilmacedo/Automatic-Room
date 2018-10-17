enum ExceptionType
{
    TIMEOUT, CONNECTION, NILL
};

struct Exception
{
    ExceptionType t;
    String desc;
    Console console;

    String format()
    {
        String error;
        int num;
        switch (t)
        {
            case TIMEOUT:
                error = "TIMEOUT";
                num = "1";
                break;
            case CONNECTION:
                error = "CONNECTION";
                num = "2";
                break;
            default:
                error = "NULL";
                num = "0";
                break;
        }

        return String("Exception(" + num + ") " + error);
    }

    void throw(ExceptionType type)
    {
        t = type;
        Console.sendMessage(format());
        return;
    }

    void throw(ExceptionType type, String desc)
    {
        t = type;
        String message = format();
        message += String(": " + desc);
        Console.sendMessage(message);
        return;
    }
};
