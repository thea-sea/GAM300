#include "Tools/EditorHistory.h"
#include <iostream>
#include <vector>
#include <functional>
//for input processing ctrl Z for undo andd shift ctrl z for redo
#include "Input/Input.h"

namespace TDS
{

    CommandHistory::CommandHistory() : currentIndex(-1) {}

    CommandHistory::~CommandHistory() {
        for (Command* command : history) {
            delete command;
        }
    }

    void CommandHistory::execute(Command* command) {
        if (currentIndex < static_cast<int>(history.size()) - 1) {
            // if we execute new command after undo, clear the redo history
            for (size_t i = currentIndex + 1; i < history.size(); i++) {
                delete history[i];
            }
            history.resize(currentIndex + 1);
        }

        command->execute();
        history.push_back(command);
        currentIndex++;
    }

    void CommandHistory::undo() {
        if (canUndo()) {
            history[currentIndex]->undo();
            currentIndex--;
        }
    }

    void CommandHistory::redo() {
        if (canRedo()) {
            currentIndex++;
            history[currentIndex]->execute();
        }
    }

    bool CommandHistory::canUndo() const {
        return currentIndex >= 0;
    }

    bool CommandHistory::canRedo() const {
        return currentIndex < static_cast<int>(history.size()) - 1;
    }

}

//how to eventually use the undo redo in main loop:
//int main() {
//    CommandHistory history;
//
//    
//    Command* command1 = new PrintCommand("Command 1");
//    history.execute(command1);
//
//    Command* command2 = new PrintCommand("Command 2");
//    history.execute(command2);
//
//    // Undo the last command
//    if (history.canUndo()) {
//        history.undo();
//    }
//
//    // Redo the undone command
//    if (history.canRedo()) {
//        history.redo();
//    }
//
//    // Clean up the allocated memory for commands
//    delete command1;
//    delete command2;
//
//    return 0;
//}
