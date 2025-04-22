#include <iostream>
#include <windows.h>
#include "PatternTemplates.h"
#include <list>

// to print chinese characters, first change fonts for cmd
// then edit command proccessor to support full unicode chcp 65001

using namespace std;


enum class LangRegion : int
{
    Europe,
    Asia,
    Eurasia,
    Unknown
};

//перегрузка "<<" для вывода enum
ostream &operator << ( ostream& os, LangRegion lang )
{
   const string name[] = { "Europe", "Asia", "Eurasia" };
   return os << name[ static_cast<int> (lang) ];
}

//Strategy
enum class CreationTimeEnum : int
{
    fifthAD,
    ninthAD,
    thirteenthBC
};

class BirthTime
{
public:
    virtual ~BirthTime() {}
    virtual void showInfo() = 0;
};

class FifthCentury : public BirthTime
{
    void showInfo() { cout << "Language was born in the 5th century AD"; }
};

class NinthCentury : public BirthTime
{
    void showInfo() { cout << "Language was born in the 9th century AD"; }
};

class ThirteenthCentury : public BirthTime
{
    void showInfo() { cout << "Language was born in the 13th century BC"; }
};

BirthTime *DefineBirthTime(CreationTimeEnum Create)
{
    switch(Create)
    {
        case CreationTimeEnum::fifthAD: return new FifthCentury;
        case CreationTimeEnum::ninthAD: return new NinthCentury;
        case CreationTimeEnum::thirteenthBC: return new ThirteenthCentury;
        default: return nullptr;
    }
}

class Languages
{
private:
    LangRegion region;
    string name;
    double NumberOfSpeakers;
    BirthTime *BirthCentury;
    void DoShowInfo()
    {
        if(BirthCentury == nullptr)
        {
            cout << name << " language: " << "Common in " << region << ", " << NumberOfSpeakers << " speakers. ";
            cout << "Time of birth: unknown" << endl;
        }
        else
        {
            cout << name << " language: " << "Common in " << region << ", " << NumberOfSpeakers << " speakers. ";
            BirthCentury->showInfo();
            cout << endl;
        }
    };

protected:
    bool WidelySpoken;
    bool HasAlphabet;

public:
    Languages(LangRegion reg, string name, double NumberOfSpeakers, bool WidelySpoken, bool HasAlphabet) : region(reg),  name(name), NumberOfSpeakers(NumberOfSpeakers),
    WidelySpoken(WidelySpoken), HasAlphabet(HasAlphabet), BirthCentury(nullptr) {}
    virtual ~Languages()
    {
        if(BirthCentury != nullptr) delete BirthCentury;
    }

    bool IsWidelySpoken() const { return WidelySpoken; }
    LangRegion GetSpeakers() const { return region; }
    bool Alphabet() const { return HasAlphabet; }

    void showInfo()
    {
        DoShowInfo();
    }

    void DetermineBirthCentury(BirthTime *Create) { BirthCentury = Create; }
};

class Chinese : public Languages
{
public:
    Chinese() : Languages(LangRegion::Asia, "Chinese", 1.35*10e9, true, false) { DetermineBirthCentury(DefineBirthTime(CreationTimeEnum::thirteenthBC)); }
    virtual ~Chinese() {}
};


class English : public Languages
{
public:
    English() : Languages(LangRegion::Europe, "English", 1.457*10e9, true, true) { DetermineBirthCentury(DefineBirthTime(CreationTimeEnum::fifthAD)); }
    virtual ~English() {}
};

class Russian : public Languages
{
public:
    Russian() : Languages(LangRegion::Europe, "Russian", 255*10e6, false, true) { DetermineBirthCentury(DefineBirthTime(CreationTimeEnum::ninthAD)); }
    virtual ~Russian() {}
};

class LanguageRegionDecorator : public IteratorDecorator<class Languages*>
{
private:
    LangRegion region;

public:
    LanguageRegionDecorator(Iterator<Languages*> *it, LangRegion reg)
    : IteratorDecorator<Languages*> (it), region(reg) {}

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->GetSpeakers() != region)
        {
            It->Next();
        }
    }
    void Next()
    {
        do
        {
            It->Next();
        } while(!It->IsDone() && It->GetCurrent()->GetSpeakers() != region);
    }
};

class LanguageSpeakerDecorator : public IteratorDecorator<class Languages*>
{
private:
    bool Spoken;

public:
    LanguageSpeakerDecorator(Iterator<Languages*> *it, bool IsWidelySpoken)
    : IteratorDecorator<Languages*> (it), Spoken(IsWidelySpoken) {}

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->IsWidelySpoken() != Spoken)
        {
            It->Next();
        }
    }
    void Next()
    {
        do
        {
            It->Next();
        } while(!It->IsDone() && It->GetCurrent()->IsWidelySpoken() != Spoken);
    }
};

class LanguageAlphabetDecorator : public IteratorDecorator<class Languages*>
{
private:
    bool Letters;

public:
    LanguageAlphabetDecorator(Iterator<Languages*> *it, bool Alphabet)
    : IteratorDecorator<Languages*> (it), Letters(Alphabet) {}

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->Alphabet() != Letters)
        {
            It->Next();
        }
    }
    void Next()
    {
        do
        {
            It->Next();
        } while(!It->IsDone() && It->GetCurrent()->Alphabet() != Letters);
    }
};

void ShowInfo(Iterator<Languages*> *it)
{
    for(it->First(); !it->IsDone(); it->Next())
    {
        Languages *currentPos = it->GetCurrent();
        currentPos->showInfo();
    }
};

enum class LanguageType : int
{
    Chinese = 1,
    English = 2,
    Russian = 3
};

Languages* DefineLanguage(LanguageType type)
{
    Languages* newLanguage = nullptr;

    if (type == LanguageType::Chinese)
    {
        newLanguage = new Chinese;
    }
    else if (type == LanguageType::English)
    {
        newLanguage = new English;
    }
    else if (type == LanguageType::Russian)
    {
        newLanguage = new Russian;
    }

    return newLanguage;
}


int main()
{
    //SetConsoleOutputCP(936);
    setlocale(LC_ALL, "");

    cout << "Enter number of elements inside the container: ";
    int N = 0;
    cin >> N;

    VectorClass<Languages*> LanguageVector;
    for (size_t i = 0; i < N; i++)
    {
        int lang_num = rand() % 3 + 1;
        LanguageType lang_type = static_cast<LanguageType>(lang_num);
        Languages* newLanguage = DefineLanguage(lang_type);
        LanguageVector.Push_back(newLanguage);
    }

    ArrayClass<Languages*> LanguageArray;
    for (size_t i = 0; i < N; i++)
    {
        int lang_num = rand() % 3 + 1;
        LanguageType lang_type = static_cast<LanguageType>(lang_num);
        Languages* newLanguage = DefineLanguage(lang_type);
        LanguageArray.Add(newLanguage);
    }

    list<Languages*> LanguageVector1;
    for(size_t i=0; i<N; i++)
        {
        int lang_num = rand() % 3 + 1;
        LanguageType lang_type = static_cast<LanguageType>(lang_num);
        Languages* newLanguage = DefineLanguage(lang_type);
        LanguageVector1.push_back(newLanguage);
        }


    cout << "Listing all wide spread languages:" << endl;
    Iterator<Languages*> *wideIt = new LanguageSpeakerDecorator(LanguageVector.GetIterator(), true);
    ShowInfo(wideIt);
    delete wideIt;
    cout << endl << endl;

    //По хорошему, чтобы русский язык учитывался и в Европе и в Азии надо либо наложить побитовые операции на enum класс, либо преобразовать enum class в string и работать уже с ним
    //но как это сделать я не понимаю
    cout << "Listing non wide spread languages with alphabet from Europe:" << endl;
    Iterator<Languages*> *allIt = new LanguageSpeakerDecorator(new LanguageAlphabetDecorator(new LanguageRegionDecorator(LanguageVector.GetIterator(), LangRegion::Europe), true), false);
    ShowInfo(allIt);
    delete allIt;
    cout << endl << endl;

    //Adapter
    cout << "Listing all languages without alphabet(with adapter):" << endl;
    Iterator<Languages*> *adaptIt = new ConstIteratorAdapter<std::list<Languages*>, Languages*>(&LanguageVector1);
    Iterator<Languages*> *alphIt = new LanguageAlphabetDecorator(adaptIt, false);
    ShowInfo(alphIt);
    delete alphIt;

}
