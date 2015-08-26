
#pragma once

class Port
{
public:
    const QString& getPortName() const { return name; }
private:
    QString name;
};
