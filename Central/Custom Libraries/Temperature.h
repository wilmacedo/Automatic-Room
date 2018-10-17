struct Temperature
{
    int p;
    float v;

    int setPin(int i)
    {
        return p = i;
    }

    int getPin()
    {
        return p;
    }

    float setVoltage(float f)
    {
        return v = f;
    }

    float getVoltage()
    {
        return v;
    }

    float getTemperature()
    {
        int og_temp = analogRead(getPin());

        return (((og_temp * getVoltage()) / 1024.0) - 0.5) * 100;
    }
};
