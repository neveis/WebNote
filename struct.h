//
// Created by root on 5/30/17.
//

#ifndef WEBNOTE_STRUCT_H
#define WEBNOTE_STRUCT_H

#include <memory>

struct NOTE_STRUCT{
    int nid;
    std::string title;
    std::string content;
};

typedef std::shared_ptr<NOTE_STRUCT> note_ptr;


#endif //WEBNOTE_STRUCT_H
