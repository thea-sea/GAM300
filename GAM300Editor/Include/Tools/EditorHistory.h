#pragma once

#ifndef COMMANDHISTORY_H
#define COMMANDHISTORY_H

#include <vector>
namespace TDS
{

    class Command {
    public:
        virtual void execute() = 0;
        virtual void undo() = 0;
    };

    class CommandHistory {
    public:
        CommandHistory();
        ~CommandHistory();

        //check for ctrl z keys to be done in imgui toolbar to do same function as undo button
        void execute(Command* command);
        void undo();
        void redo();

        bool canUndo() const;
        bool canRedo() const;

    private:
        std::vector<Command*> history;
        int currentIndex;
    };

}

#endif  // COMMANDHISTORY_H