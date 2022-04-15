#ifndef ERROR_H
#define ERROR_H

class NoRoomError
{
public:
    const char* what(){
        return "Wait, we shouldn't be here/ Lets go this way maybe?\n";
    }
};

#endif // ERROR_H
