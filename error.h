#ifndef ERROR_H
#define ERROR_H

class NoRoomError
{
public:
    const char* what(){
        return "Hang on, we shouldn't be here/ Go this way instead?\n";
    }
};

#endif // ERROR_H
