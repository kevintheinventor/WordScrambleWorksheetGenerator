#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <list>
#include <utility>
#include <cctype>

using namespace std; 
// there are 500 words
// open the document
// pick 10 words 
//create a 15 x 15 matrix
const string placeHolder = "*";

class WordScramble
{
    public:
    WordScramble();
    virtual void displayMatrix();
    int randomNumSelector();
    virtual void selectWordsForMatrix();
    void showSelectedWords();
    string invertWord(string word);
    bool fillHorizontally(string word, int attemptCount);
    bool fillVertically(string word, int attemptCount);
    bool fillDiagionally(string word, int attemptCount);
    void fillMatrixWithWords();
    void fillMatrixWithRandomLetters();
    vector <string> wordList;
    vector<string> selectedWords;
    vector<vector<string> > wordScrambledMatrix;
    private:
    int rows;
    int cols;
    int maxWordsInScrambled;
    int numOfWordsInSave;

};
WordScramble :: WordScramble()
{
    ifstream file;
    string word;
    istringstream in;
    string currentWord;
    vector <string> rawWordList;
    bool detectedDuplicate = false;
    rows = 15;
    cols = 15;
    numOfWordsInSave = 0;
    maxWordsInScrambled = 10;
    wordScrambledMatrix.resize(rows, vector<string>(cols));
    //fill matrix with placeHolder
    for (int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            wordScrambledMatrix[i][j] = placeHolder;
        }
    }
    //fill wordlist
    file.open("wordList.txt"); 
    if (!file.is_open()) //error message
    {
        cout << "error: couldn't open wordList.txt" << endl;
    }
    while (file >> word)//insert word into raw word list
    {
        rawWordList.push_back(word);
    }
    for (int i = 0; i < rawWordList.size() - 1; i++) //check for duplicate and insert into word list
    {
        currentWord = rawWordList.at(i);
        for (int j = i + 1; j < rawWordList.size(); j++)
        {
            if (currentWord == rawWordList.at(j))
            {
                detectedDuplicate = true;
            }
        }
        if (detectedDuplicate == false)
        {
            (wordList).push_back(currentWord);
        }
        detectedDuplicate = false; //reset flag
    }
    file.close();
    selectWordsForMatrix(); //select words for the scramble matrix
}
void WordScramble :: displayMatrix()
{
    for (int i = 0; i < (wordScrambledMatrix).size(); i++)
    {
        for (int j = 0; j < (wordScrambledMatrix)[i].size(); j++)
        {
            cout << (wordScrambledMatrix)[i][j] << " ";
        }
        cout << endl;
    }
}
int WordScramble :: randomNumSelector()
{
    int minNum = 0;
    int maxNum = wordList.size() - 1;
    random_device rd;
    uniform_int_distribution <int> dist(minNum, maxNum);
    return dist(rd);
}
void WordScramble :: selectWordsForMatrix()
{
    int i = 0;
    vector<int> numUsed;
    bool numberRepated = false;
    bool wordTooLargeForMatrix = false;
    int postionSelected = 0;
    while (i < maxWordsInScrambled)
    {
        do
        {
            postionSelected = randomNumSelector();
            wordTooLargeForMatrix = false;
            numberRepated = false;
            for (int j = 0; j < numUsed.size(); j++)
            {
                if (numUsed.at(j) == postionSelected)
                {
                    numberRepated = true;
                }
            }
            if (wordList.at(postionSelected).size() > rows || wordList.at(postionSelected).size() > cols)
            {
                wordTooLargeForMatrix = true;
            }
        } while (numberRepated || wordTooLargeForMatrix);
        numUsed.push_back(postionSelected);
        selectedWords.push_back(wordList.at(postionSelected));
        i++;
        numberRepated = false;
    }
}
void WordScramble :: showSelectedWords()
{
    for (int i = 0; i < selectedWords.size(); i++)
    {
        cout << i + 1 << ". " << selectedWords.at(i) << endl;
    }
}
string WordScramble :: invertWord(string word)
{
    string wordsInverted;
    for (int i = word.size() - 1; i >= 0; i--)
    {
        wordsInverted += word.at(i);
    }
    return wordsInverted;
}
bool WordScramble :: fillHorizontally(string word, int attemptCount)
{
    random_device rd;
    pair<int,int> coord;
    vector<pair<int, int> > avaliableSpots;
    int selectedRow = 0;
    int selectedCols = 0;
    int wordSize = word.size();
    //find all avaliable spaces
    for (int i = 0; i < wordScrambledMatrix.size(); i++)
    {
        selectedRow = i;
        for(int j = 0; j < wordScrambledMatrix[i].size()- word.size() + 1; j++)
        {
            bool isAvaliable = true;
            for (int k = 0; k < word.size(); k++)
            {
                if(wordScrambledMatrix[i][j + k] != placeHolder)
                {
                    isAvaliable = false;
                    break;
                }
            }
            if (isAvaliable == true)
            {
                selectedCols = j;
                coord.first = selectedRow;
                coord.second = selectedCols;
                avaliableSpots.push_back(coord);
            }
        }
    }
    if(!avaliableSpots.empty())
    {
        uniform_int_distribution <int> dist (0,avaliableSpots.size() - 1);
        int choosenLocation = dist(rd);
        selectedRow = avaliableSpots[choosenLocation].first;
        selectedCols = avaliableSpots[choosenLocation].second;
    }
    else if(attemptCount < 2)
    {
        attemptCount++;
        bool tryDiff = fillVertically(word, attemptCount);
        if (tryDiff == false)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
    for(int j = 0; j < word.size(); j ++)
    {
        if ( wordScrambledMatrix[selectedRow][selectedCols + j] == placeHolder)
        {
            wordScrambledMatrix[selectedRow][selectedCols + j] = word[j];
        }
        else
        {
            displayMatrix();
            cout << "CRITICAL ERROR: OVERLAPPING WORDS (Horizontal)" << endl;
        }
    }
    return true;
    
}
bool WordScramble :: fillVertically(string word, int attemptCount)
{
   random_device rd;
    pair<int,int> coord;
    vector<pair<int, int> > avaliableSpots;
    int selectedRow = 0;
    int selectedCols = 0;
    int wordSize = word.size();
    //find all avaliable spaces
    for(int j = 0; j < wordScrambledMatrix[0].size(); j++)
    {
        selectedCols = j;
        for (int i = 0; i < wordScrambledMatrix.size()- word.size() + 1; i++)
        {
            bool isAvaliable = true;
            
            for (int k = 0; k < word.size(); k++)
            {
                if(wordScrambledMatrix[i + k][j] != placeHolder)
                {
                    isAvaliable = false;
                    break;
                }
            }
            if (isAvaliable == true)
            {
                selectedRow = i;
                coord.first = selectedRow;
                coord.second = selectedCols;
                avaliableSpots.push_back(coord);
            }
        }
    }
    if(!avaliableSpots.empty())
    {
        uniform_int_distribution <int> dist (0,avaliableSpots.size() - 1);
        int choosenLocation = dist(rd);
        selectedRow = avaliableSpots[choosenLocation].first;
        selectedCols = avaliableSpots[choosenLocation].second;
        
    }
    else if(attemptCount < 2)
    {
        attemptCount++;
        bool tryDiff = fillDiagionally(word, attemptCount);
        if (tryDiff == false)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false; //useless, but here for safety
    }
    for(int j = 0; j < word.size(); j ++)
    {
        if (wordScrambledMatrix[selectedRow + j][selectedCols] == placeHolder)
        {
            wordScrambledMatrix[selectedRow + j][selectedCols] = word[j];
        }
        else
        {
            displayMatrix();
            cout << "CRITICAL ERROR: OVERLAPPING WORDS (Vertical)" << endl;
            cout << "CurrRow: " << selectedRow + j << endl;
        }
    }
    return true;
}
bool WordScramble :: fillDiagionally(string word, int attemptCount)
{
    random_device rd;
    pair<int,int> coord;
    vector<pair<int, int> > avaliableSpots;
    int selectedRow = 0;
    int selectedCols = 0;
    int wordSize = word.size();
    //find all avaliable spaces
    for (int i = 0; i < wordScrambledMatrix.size()- word.size() + 1; i++)
    {
        selectedRow = i;
        for(int j = 0; j < wordScrambledMatrix[i].size()- word.size() + 1; j++)
        {
            bool isAvaliable = true;
            for (int k = 0; k < word.size(); k++)
            {
                if(wordScrambledMatrix[i + k][j + k] != placeHolder)
                {
                    isAvaliable = false;
                    break;
                }
            }
            if (isAvaliable == true)
                {
                    selectedCols = j;
                    coord.first = selectedRow;
                    coord.second = selectedCols;
                    avaliableSpots.push_back(coord);
                }
                
        }
            
    }
    
    if(!avaliableSpots.empty())
    {
        uniform_int_distribution <int> dist (0,avaliableSpots.size() - 1);
        int choosenLocation = dist(rd);
        selectedRow = avaliableSpots[choosenLocation].first;
        selectedCols = avaliableSpots[choosenLocation].second;
    }
    else if(attemptCount < 2)
    {
        attemptCount++;
        bool tryDiff = fillVertically(word, attemptCount);
        if (tryDiff == false)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
    for(int j = 0; j < word.size(); j ++)
    {
        if (wordScrambledMatrix[selectedRow + j][selectedCols + j] == placeHolder)
        {
            wordScrambledMatrix[selectedRow + j][selectedCols + j] = word[j];
        }
        else
        {
            displayMatrix();
            cout << "CRITICAL ERROR: OVERLAPPING WORDS(Diagonal)" << endl;
        }
    }
    return true;
}
void WordScramble :: fillMatrixWithWords()
{
    /*
    there are a few ways.
    1. horizontal
    1a. horizontal backwards
    2. vertical
    2a. vertical backwards
    3. diagonal
    3a. diagonal backward
    */
    //for scenerio selection
    random_device rd;
    uniform_int_distribution <int> dist (1,3);
    int scenerioNum = 0;
    //for backward selection
    random_device rs;
    uniform_int_distribution <int> rang (0,1);
    bool wordBackward = false;
    string stringHolder;

   for(int i = 0; i < selectedWords.size(); i++)
   {
        scenerioNum = dist(rd);
        wordBackward = rang(rs);
        string *choosenWord = new string;
        (*choosenWord) = selectedWords.at(i);
        string currWord;
        bool ableToFill = false;
        for (char c : (*choosenWord))
        {
            currWord += toupper(c);
        }
        delete choosenWord;
        //filling in the matrix
        switch (scenerioNum)
        {
            case 1:
            if (wordBackward)
            {
                stringHolder = currWord;
                currWord = invertWord(stringHolder);
                ableToFill = fillHorizontally(currWord, 0);
            }
            else
            {
                ableToFill =fillHorizontally(currWord, 0);
            }
            break;
            case 2:
            if (wordBackward)
            {
                string stringHolder = currWord;
                currWord = invertWord(stringHolder);
                ableToFill = fillVertically(currWord, 0);
            }
            else
            {
                ableToFill = fillVertically(currWord, 0);
            }
            break;
            case 3:
            if (wordBackward)
            {
                string stringHolder = currWord;
                currWord = invertWord(stringHolder);
                ableToFill = fillDiagionally(currWord, 0);
            }
            else
            {
                ableToFill =fillDiagionally(currWord, 0);
            }
            break;
            if (ableToFill == false)
            {
                cout << "error: couldn't fit word: " << currWord << endl;
                selectedWords.erase(selectedWords.begin() + i);
                i--;
            }
        }
   }
}
void WordScramble :: fillMatrixWithRandomLetters()
{
    string alphabet[26] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
    random_device rd;
    uniform_int_distribution <int> alph (0,25);
    for (int i = 0; i < wordScrambledMatrix.size(); i++)
    {
        for (int j = 0; j < wordScrambledMatrix[i].size(); j++)
        {
            if (wordScrambledMatrix[i][j] == placeHolder)
            {
                int randomNum = alph(rd);
                string letter = alphabet[randomNum];
                wordScrambledMatrix[i][j] = letter;
            }
        }
    }
}
class Developer : public WordScramble
{
    public:
    Developer() : WordScramble(){};
    void MaxCharFrequencyInWordList();
    void showDuplicate();
    void numOfCharInSelectedWord();
    private:
};
void Developer :: MaxCharFrequencyInWordList()
{
    int currNumOfChar = 0, maxNumOfChar = 0, numOfSimilarWord = 0;

    for (string word : wordList)
    {
        currNumOfChar = word.size();
        if (currNumOfChar > maxNumOfChar)
        {
            maxNumOfChar = currNumOfChar;
        }
        else if (currNumOfChar == maxNumOfChar)
        {
            numOfSimilarWord++;
        }
    }
    cout << "Largest Size: " << maxNumOfChar << endl;
    cout << "Num of words with this size: " << numOfSimilarWord << endl;
}
void Developer :: showDuplicate()
{
    string currentWord;
    for (int i = 0; i < wordList.size() - 1; i++)
    {
        currentWord = wordList.at(i);

        for (int j = i + 1; j < wordList.size(); j++)
        {
            if (currentWord == wordList.at(j))
            {
                cout << currentWord << " " << "locatated at " << i + 1 << " & " << j + 1 << endl; 

            }
        }
    }
}
void Developer :: numOfCharInSelectedWord()
{
    int numberOfChar = 0;
    for (int i = 0; i < selectedWords.size(); i++)
    {
        numberOfChar += selectedWords.at(i).size();
    }
    cout << "Total Number of Character in Selected Words: " << numberOfChar << endl;
}
class Sheet : public WordScramble
{
    public:
    Sheet();
    string writeMatrix();
    string writeSelectedWords();
    private:
};
Sheet :: Sheet() : WordScramble()
{
    fillMatrixWithWords();
    fillMatrixWithRandomLetters();
    displayMatrix();
    cout << endl;
    cout << "Word List:" << endl;
    showSelectedWords();
}
string Sheet :: writeMatrix()
{
    ostringstream matrix;
    for (int i = 0; i < (wordScrambledMatrix).size(); i++)
        {
            for (int j = 0; j < (wordScrambledMatrix)[i].size(); j++)
            {
                matrix << (wordScrambledMatrix)[i][j] << " ";
            }
            matrix << endl;
        }
    return matrix.str();
}
string Sheet :: writeSelectedWords()
{
    ostringstream wordList;
    for (int i = 0; i < selectedWords.size(); i++)
    {
        wordList << i + 1 << ". " << selectedWords.at(i) << endl;
    }
    return wordList.str();
}

class workBook : public Sheet
{
    public:
    workBook(int numOfPages) : numOfPages(numOfPages)
    {
        ofstream outputTxtFile("SheetOutput.txt");
        ostringstream intro;
        string text = "";
        int totalWidth = 38; //of paper
        int totalHeight = 27; // of paper
        
        int middlePage = floor(totalHeight/2);

        for (int i = 0; i < 27; i ++)
        {
            if(i == middlePage - 1)
            {
                text = "Hephastus Present";
                int padding = totalWidth - text.length();
                int leftPadding = padding / 2;
                int rightPadding = padding - leftPadding;
                intro << setw(leftPadding) << "" << text << setw(rightPadding) << "" << endl;
            }
            else if(i == middlePage)
            {
                text = "Scramble Word Puzzle";
                int padding = totalWidth - text.length();
                int leftPadding = padding / 2;
                int rightPadding = padding - leftPadding;
                intro << setw(leftPadding) << "" << text << setw(rightPadding) << "" << endl;
            }
            else if(i == middlePage + 1)
            {
                text = "Vol 1";
                int padding = totalWidth - text.length();
                int leftPadding = padding / 2;
                int rightPadding = padding - leftPadding;
                intro << setw(leftPadding) << "" << text << setw(rightPadding) << "" << endl;
            }
            else
            {
                intro << endl;
            }
        }
        outputTxtFile << intro.str();
        for (int i = 0; i <= numOfPages; i++)
        {
            Sheet newSheet;
            ostringstream out;
            out << newSheet.writeMatrix() << endl << "Word List:" << endl << newSheet.writeSelectedWords();
            outputTxtFile << out.str();
        }
        outputTxtFile.close();
    } 
    private:
    int numOfPages;

};
void ExportToPdf()
{

}

int main()
{
    //hit font size 9 times
    workBook volTwo(50);
    
    return 0;
}
