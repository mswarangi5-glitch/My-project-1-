#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <ctime>

using namespace std;

// ---------- Utility: safe integer input ----------
int getValidIntInput(const string& prompt)
{
    int value;
    while (true)
    {
        cout << prompt;
        if (cin >> value)
        {
            return value;
        }
        cout << "Invalid input! Please enter a valid number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// ---------- Utility: date helpers ----------
bool isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int daysInMonth(int month, int year)
{
    static int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year)) return 29;
    if (month < 1 || month > 12) return 30; // fallback
    return days[month - 1];
}

// Adds `addDays` days to a given date and returns the resulting date
void addDaysToDate(int day, int month, int year, int addDays,
                    int& outDay, int& outMonth, int& outYear)
{
    outDay = day;
    outMonth = month;
    outYear = year;

    outDay += addDays;
    while (outDay > daysInMonth(outMonth, outYear))
    {
        outDay -= daysInMonth(outMonth, outYear);
        outMonth++;
        if (outMonth > 12)
        {
            outMonth = 1;
            outYear++;
        }
    }
}

string formatDate(int d, int m, int y)
{
    string result = "";
    if (d < 10) result += "0";
    result += to_string(d) + "/";
    if (m < 10) result += "0";
    result += to_string(m) + "/" + to_string(y);
    return result;
}

// ---------- Data structure for one saved cycle record ----------
// Now stores the user's Name, Email and Age with every record,
// so history keeps a full snapshot, not just the cycle data.
struct PeriodRecord
{
    string name;
    string email;
    int age;

    int lastDay, lastMonth, lastYear;
    int cycleLength;
    int periodDuration;
    int nextDay, nextMonth, nextYear;
    string symptoms;
};

// ---------- Main tracker class ----------
class HerCycleTracker
{
private:
    string email;
    string name;
    int dobDay = 0, dobMonth = 0, dobYear = 0;
    int age = 0;

    vector<PeriodRecord> history;

    bool isValidEmail(const string& e)
    {
        bool hasAt = e.find('@') != string::npos;
        bool hasDot = e.find('.') != string::npos;
        return hasAt && hasDot;
    }

    int calculateAge(int day, int month, int year)
    {
        time_t t = time(nullptr);
        tm* now = localtime(&t);
        int curYear = now->tm_year + 1900;
        int curMonth = now->tm_mon + 1;
        int curDay = now->tm_mday;

        int calculatedAge = curYear - year;
        if (curMonth < month || (curMonth == month && curDay < day))
        {
            calculatedAge--;
        }
        return calculatedAge < 0 ? 0 : calculatedAge;
    }

    // Any age is accepted here (no upper/lower cap) - just guards
    // against blank/non-numeric input via getValidIntInput, and
    // against obviously invalid negative values.
    int getValidAgeInput(const string& prompt)
    {
        int value;
        while (true)
        {
            value = getValidIntInput(prompt);
            if (value >= 0)
            {
                return value;
            }
            cout << "Age cannot be negative. Please try again.\n";
        }
    }

public:

    // Step: Open the System (login with email id)
    void loginWithEmail()
    {
        cout << "\n---- LOGIN ----\n";
        while (true)
        {
            cout << "Enter Email ID: ";
            cin >> email;
            if (isValidEmail(email))
            {
                cout << "Login successful. Welcome, " << email << "!\n";
                break;
            }
            cout << "Invalid email format. Please try again.\n";
        }
    }

    // Step: Enter User Details (name + DOB -> derive age, OR enter age directly)
    // Any age is now allowed - the user is no longer forced to go
    // through DOB calculation only.
    void enterUserDetails()
    {
        cout << "\n---- USER DETAILS ----\n";
        cout << "Enter Name: ";
        cin >> name;

        cout << "\nHow would you like to provide your age?\n";
        cout << "  1. Enter Date of Birth (age will be calculated)\n";
        cout << "  2. Enter Age directly (any age)\n";
        int choice = getValidIntInput("Choose an option (1/2): ");

        if (choice == 2)
        {
            age = getValidAgeInput("Enter Age: ");
            // DOB left as 0/0/0 since it wasn't provided
            dobDay = dobMonth = dobYear = 0;
        }
        else
        {
            cout << "Enter Date of Birth\n";
            dobDay = getValidIntInput("Day: ");
            dobMonth = getValidIntInput("Month: ");
            dobYear = getValidIntInput("Year: ");

            age = calculateAge(dobDay, dobMonth, dobYear);
            cout << "Calculated Age: " << age << " years\n";
        }
    }

    // Steps: Enter Last Period Date -> Enter Cycle Information ->
    //        Calculate Cycle Information -> Estimate Next Period Date ->
    //        Enter Symptoms -> Save Period Record
    void enterNewCycleData()
    {
        cout << "\n---- LAST PERIOD DATE ----\n";
        int day = getValidIntInput("Day: ");
        int month = getValidIntInput("Month: ");
        int year = getValidIntInput("Year: ");

        cout << "\n---- CYCLE INFORMATION ----\n";
        int cycleLength = getValidIntInput("Enter Average Cycle Length (days): ");
        int periodDuration = getValidIntInput("Enter Period Duration (days): ");

        // Calculate cycle information
        cout << "\nCalculating cycle information...\n";
        if (cycleLength <= 0) cycleLength = 28;     // sensible default
        if (periodDuration <= 0) periodDuration = 5; // sensible default

        // Estimate next period date
        int nextDay, nextMonth, nextYear;
        addDaysToDate(day, month, year, cycleLength, nextDay, nextMonth, nextYear);

        cout << "Estimated Next Period Date: "
             << formatDate(nextDay, nextMonth, nextYear) << "\n";

        // Enter symptoms
        cout << "\n---- SYMPTOMS ----\n";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter Symptoms (comma separated, or 'none'): ";
        string symptoms;
        getline(cin, symptoms);

        // Save period record (now includes name, email, age)
        PeriodRecord record;
        record.name = name;
        record.email = email;
        record.age = age;
        record.lastDay = day;
        record.lastMonth = month;
        record.lastYear = year;
        record.cycleLength = cycleLength;
        record.periodDuration = periodDuration;
        record.nextDay = nextDay;
        record.nextMonth = nextMonth;
        record.nextYear = nextYear;
        record.symptoms = symptoms;

        history.push_back(record);
        cout << "\nPeriod record saved successfully!\n";
    }

    // Step: Display Information (most recent record)
    void displayInformation()
    {
        cout << "\n========== CYCLE INFORMATION ==========\n";
        if (history.empty())
        {
            cout << "No cycle data recorded yet.\n";
            return;
        }

        const PeriodRecord& r = history.back();
        cout << "Name: " << r.name << " (Age: " << r.age << ")\n";
        cout << "Email: " << r.email << "\n";
        cout << "Last Period Date: " << formatDate(r.lastDay, r.lastMonth, r.lastYear) << "\n";
        cout << "Cycle Length: " << r.cycleLength << " days\n";
        cout << "Period Duration: " << r.periodDuration << " days\n";
        cout << "Estimated Next Period Date: "
             << formatDate(r.nextDay, r.nextMonth, r.nextYear) << "\n";
        cout << "Symptoms: " << r.symptoms << "\n";
        cout << "========================================\n";
    }

    // Step: View History (now shows Name, Email and Age per record)
    void viewHistory()
    {
        cout << "\n========== CYCLE HISTORY ==========\n";
        if (history.empty())
        {
            cout << "No history available yet.\n";
            return;
        }

        for (size_t i = 0; i < history.size(); i++)
        {
            const PeriodRecord& r = history[i];
            cout << "\nRecord #" << (i + 1) << "\n";
            cout << "  Name          : " << r.name << "\n";
            cout << "  Email         : " << r.email << "\n";
            cout << "  Age           : " << r.age << "\n";
            cout << "  Last Period   : " << formatDate(r.lastDay, r.lastMonth, r.lastYear) << "\n";
            cout << "  Cycle Length  : " << r.cycleLength << " days\n";
            cout << "  Next Period   : " << formatDate(r.nextDay, r.nextMonth, r.nextYear) << "\n";
            cout << "  Symptoms      : " << r.symptoms << "\n";
        }
        cout << "====================================\n";
    }
};

int main()
{
    // Entry
    cout << "========================================\n";
    cout << "        WELCOME TO HER CYCLE\n";
    cout << "========================================\n";

    // Start
    cout << "\nHer Cycle will track your periods cycle.\n";

    HerCycleTracker tracker;

    // Open the System (login)
    tracker.loginWithEmail();

    // Enter User Details (name + DOB or direct age)
    tracker.enterUserDetails();

    bool continueTracking = true;
    while (continueTracking)
    {
        // Last Period Date -> Cycle Info -> Calculate -> Estimate ->
        // Symptoms -> Save Record
        tracker.enterNewCycleData();

        // Display Information
        tracker.displayInformation();

        // View History
        tracker.viewHistory();

        // Continue Tracking?
        char choice;
        cout << "\nContinue Tracking? (Y/N): ";
        cin >> choice;

        if (choice == 'y' || choice == 'Y')
        {
            continueTracking = true; // loop back to Enter New Data
        }
        else
        {
            continueTracking = false; // END
        }
    }

    cout << "\nThank you for using Her Cycle. Take care!\n";
    return 0;
}