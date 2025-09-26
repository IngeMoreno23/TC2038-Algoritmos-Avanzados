#pragma once
#include <vector>

class App;

class IMenuStrategy {
protected:
	App* app;

public:
    IMenuStrategy(App* app);
    virtual ~IMenuStrategy() = default;

    virtual std::string menuSelectionText() = 0;
    virtual void execute() = 0;
};

/*
Create Huffman Tree Menu (input):
- Create Tree from .txt File
- Create Tree from .bin File
- Create Tree from Terminal Input (plain text)
*/
class CreateTreeFromTxtFileStrategy : public IMenuStrategy {
public:
    CreateTreeFromTxtFileStrategy(App* app);

    std::string menuSelectionText() override;
    void execute() override;
};

class CreateTreeFromBinFileStrategy : public IMenuStrategy {
public:
    CreateTreeFromBinFileStrategy(App* app);

    std::string menuSelectionText() override;
    void execute() override;
};

class CreateTreeFromInputStrategy : public IMenuStrategy {
public:
    CreateTreeFromInputStrategy(App* app);

    std::string menuSelectionText() override;
    void execute() override;
};

class CreateTreeStrategy : public IMenuStrategy {
private:
    std::vector<IMenuStrategy*> strategies;
public:
    CreateTreeStrategy(App* app);
    ~CreateTreeStrategy() override;

    std::string menuSelectionText() override;
    void execute() override;
};

/*
Encode Decode File/Terminal Input Menu (output):
- Encode File .txt to .bin
- Decode File .bin to .txt (Destructive - Generates new Tree from header :'v)
- Encode Terminal Input (shows in console)
- Decode Terminal Input (shows in console)
*/
class EncodeFileStrategy : public IMenuStrategy {
public:
    EncodeFileStrategy(App* app);

    std::string menuSelectionText() override;
    void execute() override;
};

class DecodeFileStrategy : public IMenuStrategy {
public:
    DecodeFileStrategy(App* app);

    std::string menuSelectionText() override;
    void execute() override;
};

class EncodeInputStrategy : public IMenuStrategy {
public:
    EncodeInputStrategy(App* app);

    std::string menuSelectionText() override;
    void execute() override;
};
class DecodeInputStrategy : public IMenuStrategy {
public:
    DecodeInputStrategy(App* app);

    std::string menuSelectionText() override;
    void execute() override;
};

class EncodeDecodeInputStrategy : public IMenuStrategy {
private:
    std::vector<IMenuStrategy*> strategies;
public:
    EncodeDecodeInputStrategy(App* app);
    ~EncodeDecodeInputStrategy() override;

    std::string menuSelectionText() override;
    void execute() override;
};

/*
Visualization Menu:
- Print Table
- Print Current Huffman Tree
*/
class PrintTableStrategy : public IMenuStrategy {
public:
    PrintTableStrategy(App* app);

    std::string menuSelectionText() override;
    void execute() override;
};

class PrintTreeStrategy : public IMenuStrategy {
public:
    PrintTreeStrategy(App* app);

    std::string menuSelectionText() override;
    void execute() override;
};

class VisualizationStrategy : public IMenuStrategy {
private:
    std::vector<IMenuStrategy*> strategies;
public:
    VisualizationStrategy(App* app);
    ~VisualizationStrategy() override;

    std::string menuSelectionText() override;
    void execute() override;
};