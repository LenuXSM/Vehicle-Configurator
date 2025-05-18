#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <filesystem>

// Console color definitions
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[37m"
#define COLOR_BOLD    "\033[1m"

// Helper function to display headers
void printHeader(const std::string& text) {
    std::cout << COLOR_BOLD << COLOR_BLUE << "\n╔══════════════════════════════════════════════════════════╗" << COLOR_RESET << std::endl;
    std::cout << COLOR_BOLD << COLOR_BLUE << "║ " << std::left << std::setw(52) << text << " ║" << COLOR_RESET << std::endl;
    std::cout << COLOR_BOLD << COLOR_BLUE << "╚══════════════════════════════════════════════════════════╝" << COLOR_RESET << std::endl;
}

// Helper function to display menu items
void printMenuItem(int number, const std::string& text) {
    std::cout << COLOR_CYAN << " [" << number << "] " << COLOR_RESET << text << std::endl;
}

// Helper function to format prices
std::string formatPrice(double price) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << price;
    std::string priceStr = ss.str();

    // Adding thousands separators
    int pos = priceStr.find('.');
    if (pos == std::string::npos) pos = priceStr.length();

    while (pos > 3) {
        pos -= 3;
        priceStr.insert(pos, ",");
    }

    return priceStr + " USD";
}

// Helper function for loading animation
void showLoadingAnimation(const std::string& message, int duration = 1) {
    std::cout << message;
    for (int i = 0; i < 3; i++) {
        for (char c : {'.', '.', '.'}) {
            std::cout << c << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }
        std::cout << "\b\b\b   \b\b\b" << std::flush;
    }
    std::cout << COLOR_GREEN << "Done!" << COLOR_RESET << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(duration));
}

// Function to clear console screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Equipment categories
enum class EquipmentCategory {
    COMFORT,
    SAFETY,
    MULTIMEDIA,
    EXTERIOR,
    PERFORMANCE
};

// Convert category to string
std::string categoryToString(EquipmentCategory category) {
    switch (category) {
        case EquipmentCategory::COMFORT: return "Comfort";
        case EquipmentCategory::SAFETY: return "Safety";
        case EquipmentCategory::MULTIMEDIA: return "Multimedia";
        case EquipmentCategory::EXTERIOR: return "Exterior";
        case EquipmentCategory::PERFORMANCE: return "Performance";
        default: return "Other";
    }
}
// Class representing equipment option
class Equipment {
private:
    std::string name;
    std::string description;
    double price;
    EquipmentCategory category;

public:
    Equipment(const std::string& name, const std::string& description, double price, EquipmentCategory category)
        : name(name), description(description), price(price), category(category) {}

    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    double getPrice() const { return price; }
    EquipmentCategory getCategory() const { return category; }
};

// Class representing engine
class Engine {
private:
    std::string name;
    double capacity;
    int horsePower;
    std::string fuelType;
    double price;
    int co2Emissions; // Added CO2 emissions in g/km
    double fuelConsumption; // Added fuel consumption in l/100km

public:
    Engine(const std::string& name, double capacity, int horsePower,
           const std::string& fuelType, double price, int co2Emissions = 0, double fuelConsumption = 0.0)
        : name(name), capacity(capacity), horsePower(horsePower),
          fuelType(fuelType), price(price), co2Emissions(co2Emissions), fuelConsumption(fuelConsumption) {}

    std::string getName() const { return name; }
    double getCapacity() const { return capacity; }
    int getHorsePower() const { return horsePower; }
    std::string getFuelType() const { return fuelType; }
    double getPrice() const { return price; }
    int getCO2Emissions() const { return co2Emissions; }
    double getFuelConsumption() const { return fuelConsumption; }
};
// Base class for all vehicles
class Vehicle {
protected:
    std::string brand;
    std::string model;
    double basePrice;
    std::shared_ptr<Engine> engine;
    std::vector<Equipment> selectedEquipment;
    std::string color;
    std::string year;
    double discount; // Percentage discount

    // ASCII art for visualization
    std::vector<std::string> asciiArt;

public:
    Vehicle(const std::string& brand, const std::string& model, double basePrice, const std::string& year = "2023")
        : brand(brand), model(model), basePrice(basePrice), color("White"), year(year), discount(0.0) {}

    virtual ~Vehicle() = default;

    // Getters
    std::string getBrand() const { return brand; }
    std::string getModel() const { return model; }
    double getBasePrice() const { return basePrice; }
    std::string getColor() const { return color; }
    std::string getYear() const { return year; }
    double getDiscount() const { return discount; }
    const std::vector<Equipment>& getSelectedEquipment() const { return selectedEquipment; } // Added accessor

    // Setters
    void setEngine(std::shared_ptr<Engine> newEngine) { engine = newEngine; }
    void setColor(const std::string& newColor) { color = newColor; }
    void setDiscount(double newDiscount) { discount = newDiscount; }

    // Adding equipment
    void addEquipment(const Equipment& equipment) {
        // Check if equipment is already added
        auto it = std::find_if(selectedEquipment.begin(), selectedEquipment.end(),
            [&equipment](const Equipment& e) { return e.getName() == equipment.getName(); });

        if (it == selectedEquipment.end()) {
            selectedEquipment.push_back(equipment);
            std::cout << COLOR_GREEN << "✓ " << equipment.getName() << " added to configuration." << COLOR_RESET << std::endl;
        } else {
            std::cout << COLOR_YELLOW << "! " << equipment.getName() << " is already in your configuration." << COLOR_RESET << std::endl;
        }
    }

    // Removing equipment
    void removeEquipment(const std::string& name) {
        auto it = std::find_if(selectedEquipment.begin(), selectedEquipment.end(),
            [&name](const Equipment& e) { return e.getName() == name; });

        if (it != selectedEquipment.end()) {
            selectedEquipment.erase(it);
            std::cout << COLOR_RED << "✓ " << name << " removed from configuration." << COLOR_RESET << std::endl;
        } else {
            std::cout << COLOR_YELLOW << "! " << name << " is not in your configuration." << COLOR_RESET << std::endl;
        }
    }

    // Calculating total price
    virtual double calculateTotalPrice() const {
        double total = basePrice;

        if (engine) {
            total += engine->getPrice();
        }

        for (const auto& equipment : selectedEquipment) {
            total += equipment.getPrice();
        }

        // Apply discount if any
        if (discount > 0) {
            total = total * (1.0 - discount / 100.0);
        }

        return total;
    }
    // Displaying vehicle information
    virtual void displayInfo() const {
        printHeader(brand + " " + model + " (" + year + ")");

        std::cout << COLOR_BOLD << "Color: " << COLOR_RESET << color << std::endl;
        std::cout << COLOR_BOLD << "Base price: " << COLOR_RESET << formatPrice(basePrice) << std::endl;

        if (engine) {
            std::cout << COLOR_BOLD << "\nEngine: " << COLOR_RESET << engine->getName() << std::endl;
            std::cout << "  ├─ Capacity: " << engine->getCapacity() << "L" << std::endl;
            std::cout << "  ├─ Power: " << engine->getHorsePower() << " HP" << std::endl;
            std::cout << "  ├─ Fuel type: " << engine->getFuelType() << std::endl;

            if (engine->getCO2Emissions() > 0) {
                std::cout << "  ├─ CO2 emissions: " << engine->getCO2Emissions() << " g/km" << std::endl;
            }

            if (engine->getFuelConsumption() > 0) {
                std::cout << "  ├─ Fuel consumption: " << engine->getFuelConsumption() << " l/100km" << std::endl;
            }

            std::cout << "  └─ Price: " << formatPrice(engine->getPrice()) << std::endl;
        }

        if (!selectedEquipment.empty()) {
            std::cout << COLOR_BOLD << "\nSelected equipment:" << COLOR_RESET << std::endl;

            // Group equipment by category
            std::map<EquipmentCategory, std::vector<Equipment>> equipmentByCategory;
            for (const auto& equipment : selectedEquipment) {
                equipmentByCategory[equipment.getCategory()].push_back(equipment);
            }

            for (const auto& categoryPair : equipmentByCategory) {
                std::cout << COLOR_YELLOW << "  " << categoryToString(categoryPair.first) << ":" << COLOR_RESET << std::endl;

                double categoryTotal = 0.0;
                for (const auto& equipment : categoryPair.second) {
                    std::cout << "    ├─ " << equipment.getName() << ": " << formatPrice(equipment.getPrice()) << std::endl;
                    std::cout << "    │  " << equipment.getDescription() << std::endl;
                    categoryTotal += equipment.getPrice();
                }
                std::cout << "    └─ " << COLOR_BOLD << "Category total: " << formatPrice(categoryTotal) << COLOR_RESET << std::endl;
            }
        }

        if (discount > 0) {
            double discountAmount = (basePrice + (engine ? engine->getPrice() : 0.0));
            for (const auto& equipment : selectedEquipment) {
                discountAmount += equipment.getPrice();
            }
            discountAmount = discountAmount * (discount / 100.0);

            std::cout << COLOR_BOLD << "\nDiscount: " << COLOR_RESET << discount << "% (" << formatPrice(discountAmount) << ")" << std::endl;
        }

        std::cout << COLOR_BOLD << COLOR_GREEN << "\nTotal price: " << formatPrice(calculateTotalPrice()) << COLOR_RESET << std::endl;
    }

    // Vehicle visualization
    virtual void visualize() const {
        clearScreen();
        printHeader("Visualization of " + brand + " " + model + " in " + color + " color");

        // Apply color to ASCII art
        std::string colorCode;
        if (color == "Red") colorCode = COLOR_RED;
        else if (color == "Blue") colorCode = COLOR_BLUE;
        else if (color == "Green") colorCode = COLOR_GREEN;
        else if (color == "Yellow") colorCode = COLOR_YELLOW;
        else if (color == "Black") colorCode = COLOR_BOLD;
        else colorCode = COLOR_WHITE; // Default for other colors

        for (const auto& line : asciiArt) {
            std::cout << colorCode << line << COLOR_RESET << std::endl;
        }

        std::cout << "\nPress Enter to continue...";
        std::cin.ignore();
        std::cin.get();
    }
    // Saving configuration to file
    virtual void saveToFile(const std::string& filename) const {
        // Create directory if it doesn't exist
        std::filesystem::path dirPath = "configs";
        if (!std::filesystem::exists(dirPath)) {
            std::filesystem::create_directory(dirPath);
        }

        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << COLOR_RED << "Cannot open file for writing: " << filename << COLOR_RESET << std::endl;
            return;
        }

        file << "VEHICLE_CONFIGURATION\n";
        file << "VERSION 2.0\n";
        file << "DATE " << getCurrentDateTime() << "\n\n";

        file << "[VEHICLE]\n";
        file << "BRAND=" << brand << "\n";
        file << "MODEL=" << model << "\n";
        file << "YEAR=" << year << "\n";
        file << "BASE_PRICE=" << basePrice << "\n";
        file << "COLOR=" << color << "\n";
        file << "DISCOUNT=" << discount << "\n\n";

        if (engine) {
            file << "[ENGINE]\n";
            file << "NAME=" << engine->getName() << "\n";
            file << "CAPACITY=" << engine->getCapacity() << "\n";
            file << "HORSEPOWER=" << engine->getHorsePower() << "\n";
            file << "FUEL_TYPE=" << engine->getFuelType() << "\n";
            file << "PRICE=" << engine->getPrice() << "\n";
            file << "CO2_EMISSIONS=" << engine->getCO2Emissions() << "\n";
            file << "FUEL_CONSUMPTION=" << engine->getFuelConsumption() << "\n\n";
        }

        file << "[EQUIPMENT]\n";
        file << "COUNT=" << selectedEquipment.size() << "\n\n";

        for (size_t i = 0; i < selectedEquipment.size(); ++i) {
            file << "[EQUIPMENT_ITEM_" << i + 1 << "]\n";
            file << "NAME=" << selectedEquipment[i].getName() << "\n";
            file << "DESCRIPTION=" << selectedEquipment[i].getDescription() << "\n";
            file << "PRICE=" << selectedEquipment[i].getPrice() << "\n";
            file << "CATEGORY=" << static_cast<int>(selectedEquipment[i].getCategory()) << "\n\n";
        }

        file << "[SUMMARY]\n";
        file << "TOTAL_PRICE=" << calculateTotalPrice() << "\n";

        file.close();
        showLoadingAnimation("Saving configuration to " + filename);
    }

    // Helper function to get current date and time
    std::string getCurrentDateTime() const {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
};
// Class for passenger cars
class Car : public Vehicle {
private:
    int numberOfDoors;
    std::string bodyType; // sedan, hatchback, SUV, etc.
    int trunkCapacity; // in liters

public:
    Car(const std::string& brand, const std::string& model, double basePrice,
        int numberOfDoors, const std::string& bodyType, int trunkCapacity = 0,
        const std::string& year = "2023")
        : Vehicle(brand, model, basePrice, year), numberOfDoors(numberOfDoors),
          bodyType(bodyType), trunkCapacity(trunkCapacity) {

        // Enhanced ASCII visualization for car
        if (bodyType == "Sedan") {
            asciiArt = {
                "          ______--------___",
                "         /|             / |",
                "        / |  ___      /   |",
                "       /__|_/   \\____/    |",
                "      |            |     _|",
                "      |____________|____/",
                "      |            |",
                "      \\____________/",
                "       O        O"
            };
        } else if (bodyType == "Hatchback") {
            asciiArt = {
                "         __---~~~~--__",
                "       /|             \\",
                "      / |  ___        |",
                "     /__|_/   \\____   |",
                "    |            |   _|",
                "    |____________|__/",
                "    |            |",
                "    \\____________/",
                "     O        O"
            };
        } else if (bodyType == "SUV") {
            asciiArt = {
                "         __---~~~~--__",
                "       /|             \\",
                "      / |  ___        |",
                "     /__|_/   \\____   |",
                "    |            |    |",
                "    |            |    |",
                "    |____________|____|",
                "    |            |",
                "    \\____________/",
                "     O        O"
            };
        } else {
            asciiArt = {
                "    ____",
                " __/  |_\\_",
                "|  _     _`-.",
                "'-(_)---(_)--'"
            };
        }
    }

    int getNumberOfDoors() const { return numberOfDoors; }
    std::string getBodyType() const { return bodyType; }
    int getTrunkCapacity() const { return trunkCapacity; }

    void displayInfo() const override {
        Vehicle::displayInfo();
        std::cout << COLOR_BOLD << "\nCar details:" << COLOR_RESET << std::endl;
        std::cout << "  ├─ Body type: " << bodyType << std::endl;
        std::cout << "  ├─ Number of doors: " << numberOfDoors << std::endl;
        if (trunkCapacity > 0) {
            std::cout << "  └─ Trunk capacity: " << trunkCapacity << " liters" << std::endl;
        }
    }
};
// Class for motorcycles
class Motorcycle : public Vehicle {
private:
    std::string type; // sport, cruiser, enduro, etc.
    int engineDisplacement; // in cc

public:
    Motorcycle(const std::string& brand, const std::string& model, double basePrice,
              const std::string& type, int engineDisplacement = 0,
              const std::string& year = "2023")
        : Vehicle(brand, model, basePrice, year), type(type), engineDisplacement(engineDisplacement) {

        // Enhanced ASCII visualization for motorcycle based on type
        if (type == "Sport") {
            asciiArt = {
                "    ,_oo",
                ".-/c-//:::",
                "(_)'  \\\\:::",
                "      `\\:::",
                "       _\\::",
                "      /  \\:",
                "     /    \\",
                "    (    0 )",
                "     \\    /",
                "      \\__/"
            };
        } else if (type == "Cruiser") {
            asciiArt = {
                "      ,_",
                "  .-_-' `--'",
                " /     o   \\",
                "(_)/-(_)--(_)"
            };
        } else {
            asciiArt = {
                "    ,_oo",
                ".-/c-//:::",
                "(_)'  \\\\:::",
                "      `\\:::"
            };
        }
    }

    std::string getType() const { return type; }
    int getEngineDisplacement() const { return engineDisplacement; }

    void displayInfo() const override {
        Vehicle::displayInfo();
        std::cout << COLOR_BOLD << "\nMotorcycle details:" << COLOR_RESET << std::endl;
        std::cout << "  ├─ Type: " << type << std::endl;
        if (engineDisplacement > 0) {
            std::cout << "  └─ Engine displacement: " << engineDisplacement << " cc" << std::endl;
        }
    }
};

// New class for electric vehicles
class ElectricVehicle : public Vehicle {
private:
    int batteryCapacity; // in kWh
    int range; // in km
    int chargingTime; // in minutes (for fast charging)

public:
    ElectricVehicle(const std::string& brand, const std::string& model, double basePrice,
                    int batteryCapacity, int range, int chargingTime,
                    const std::string& year = "2023")
        : Vehicle(brand, model, basePrice, year),
          batteryCapacity(batteryCapacity), range(range), chargingTime(chargingTime) {

        // ASCII art for electric vehicle
        asciiArt = {
            "      ____",
            "    /|    \\",
            "   / |     \\",
            "  /__|______\\",
            " |           |",
            " |___________|",
            " |_|       |_|",
            "   ⚡       ⚡"
        };
    }

    int getBatteryCapacity() const { return batteryCapacity; }
    int getRange() const { return range; }
    int getChargingTime() const { return chargingTime; }

    void displayInfo() const override {
        Vehicle::displayInfo();
        std::cout << COLOR_BOLD << "\nElectric vehicle details:" << COLOR_RESET << std::endl;
        std::cout << "  ├─ Battery capacity: " << batteryCapacity << " kWh" << std::endl;
        std::cout << "  ├─ Range: " << range << " km" << std::endl;
        std::cout << "  └─ Fast charging time: " << chargingTime << " minutes" << std::endl;
    }
};
// Class for configuring vehicles
class VehicleConfigurator {
private:
    std::vector<std::shared_ptr<Vehicle>> availableVehicles;
    std::vector<std::shared_ptr<Engine>> availableEngines;
    std::vector<Equipment> availableEquipment;
    std::vector<std::string> availableColors;
    std::shared_ptr<Vehicle> currentVehicle;
    std::shared_ptr<Vehicle> comparisonVehicle; // For comparing configurations

public:
    VehicleConfigurator() {
        initializeData();
    }

    // Data initialization - available vehicles, engines, equipment, colors
    void initializeData() {
        // Adding available colors
        availableColors = {"White", "Black", "Red", "Blue", "Silver", "Green", "Yellow", "Orange", "Purple", "Brown"};

        // Adding available engines with more details
        availableEngines.push_back(std::make_shared<Engine>("1.4 TSI", 1.4, 150, "Gasoline", 12000, 130, 6.5));
        availableEngines.push_back(std::make_shared<Engine>("1.6 TDI", 1.6, 115, "Diesel", 15000, 110, 4.8));
        availableEngines.push_back(std::make_shared<Engine>("2.0 TDI", 2.0, 190, "Diesel", 20000, 135, 5.2));
        availableEngines.push_back(std::make_shared<Engine>("2.0 TSI", 2.0, 220, "Gasoline", 22000, 155, 7.1));
        availableEngines.push_back(std::make_shared<Engine>("Electric Motor", 0.0, 204, "Electric", 30000, 0, 0.0));
        availableEngines.push_back(std::make_shared<Engine>("Hybrid 1.8", 1.8, 180, "Hybrid", 25000, 95, 4.2));
        availableEngines.push_back(std::make_shared<Engine>("3.0 V6", 3.0, 340, "Gasoline", 35000, 190, 9.8));
        availableEngines.push_back(std::make_shared<Engine>("650cc Twin", 0.65, 75, "Gasoline", 8000, 90, 3.8));

        // Adding available equipment with categories
        availableEquipment.push_back(Equipment("Leather upholstery", "High-quality leather upholstery", 5000, EquipmentCategory::COMFORT));
        availableEquipment.push_back(Equipment("Navigation system", "Advanced GPS navigation system", 3000, EquipmentCategory::MULTIMEDIA));
        availableEquipment.push_back(Equipment("Panoramic roof", "Glass panoramic roof", 7000, EquipmentCategory::EXTERIOR));
        availableEquipment.push_back(Equipment("Heated seats", "Heated front seats", 2000, EquipmentCategory::COMFORT));
        availableEquipment.push_back(Equipment("Premium audio system", "Audio system with 12 speakers", 4500, EquipmentCategory::MULTIMEDIA));
        availableEquipment.push_back(Equipment("Parking assistant", "Automatic parking assistant", 3500, EquipmentCategory::SAFETY));
        availableEquipment.push_back(Equipment("Adaptive cruise control", "Cruise control with adaptive function", 4000, EquipmentCategory::SAFETY));
        availableEquipment.push_back(Equipment("Backup camera", "HD camera with 360-degree view", 2500, EquipmentCategory::SAFETY));
        availableEquipment.push_back(Equipment("Sport suspension", "Lowered sport suspension", 3800, EquipmentCategory::PERFORMANCE));
        availableEquipment.push_back(Equipment("Alloy wheels 19\"", "19-inch alloy wheels", 4200, EquipmentCategory::EXTERIOR));
        availableEquipment.push_back(Equipment("LED headlights", "Full LED headlights with dynamic turn signals", 3200, EquipmentCategory::EXTERIOR));
        availableEquipment.push_back(Equipment("Sport exhaust", "Sport exhaust system with enhanced sound", 5500, EquipmentCategory::PERFORMANCE));
        availableEquipment.push_back(Equipment("Wireless charging", "Wireless phone charging pad", 800, EquipmentCategory::MULTIMEDIA));
        availableEquipment.push_back(Equipment("Head-up display", "Information projected onto windshield", 2800, EquipmentCategory::MULTIMEDIA));
        availableEquipment.push_back(Equipment("Keyless entry", "Keyless entry and start system", 1500, EquipmentCategory::COMFORT));

        // Adding available vehicles
        availableVehicles.push_back(std::make_shared<Car>("Volkswagen", "Golf", 80000, 5, "Hatchback", 380, "2023"));
        availableVehicles.push_back(std::make_shared<Car>("Audi", "A4", 150000, 4, "Sedan", 480, "2023"));
        availableVehicles.push_back(std::make_shared<Car>("BMW", "X5", 250000, 5, "SUV", 650, "2023"));
        availableVehicles.push_back(std::make_shared<Car>("Toyota", "Corolla", 90000, 4, "Sedan", 470, "2023"));
        availableVehicles.push_back(std::make_shared<Car>("Mercedes-Benz", "C-Class", 170000, 4, "Sedan", 455, "2023"));
        availableVehicles.push_back(std::make_shared<Car>("Ford", "Mustang", 220000, 2, "Coupe", 408, "2023"));
        availableVehicles.push_back(std::make_shared<Motorcycle>("Yamaha", "MT-07", 35000, "Naked", 689, "2023"));
        availableVehicles.push_back(std::make_shared<Motorcycle>("Honda", "CBR650R", 42000, "Sport", 649, "2023"));
        availableVehicles.push_back(std::make_shared<Motorcycle>("Harley-Davidson", "Fat Boy", 85000, "Cruiser", 1868, "2023"));
        availableVehicles.push_back(std::make_shared<ElectricVehicle>("Tesla", "Model 3", 180000, 75, 560, 30, "2023"));
        availableVehicles.push_back(std::make_shared<ElectricVehicle>("Nissan", "Leaf", 120000, 62, 385, 40, "2023"));
    }
    // Displaying available vehicles with improved formatting
    void displayAvailableVehicles() const {
        printHeader("Available Vehicles");

        // Group vehicles by type
        std::map<std::string, std::vector<std::shared_ptr<Vehicle>>> vehiclesByType;

        for (const auto& vehicle : availableVehicles) {
            std::string type;
            if (std::dynamic_pointer_cast<Car>(vehicle)) {
                type = "Cars";
            } else if (std::dynamic_pointer_cast<Motorcycle>(vehicle)) {
                type = "Motorcycles";
            } else if (std::dynamic_pointer_cast<ElectricVehicle>(vehicle)) {
                type = "Electric Vehicles";
            } else {
                type = "Other";
            }
            vehiclesByType[type].push_back(vehicle);
        }

        size_t index = 1;
        for (const auto& typePair : vehiclesByType) {
            std::cout << COLOR_YELLOW << "\n" << typePair.first << ":" << COLOR_RESET << std::endl;

            for (const auto& vehicle : typePair.second) {
                std::cout << COLOR_CYAN << " [" << index << "] " << COLOR_RESET;
                std::cout << vehicle->getBrand() << " " << vehicle->getModel() << " (" << vehicle->getYear() << ") - ";
                std::cout << formatPrice(vehicle->getBasePrice()) << std::endl;
                index++;
            }
        }
    }

    // Vehicle selection with improved feedback
    bool selectVehicle(size_t index) {
        if (index >= 1 && index <= availableVehicles.size()) {
            currentVehicle = availableVehicles[index - 1];
            showLoadingAnimation("Selecting vehicle");
            std::cout << COLOR_GREEN << "✓ You've selected: " << currentVehicle->getBrand() << " "
                      << currentVehicle->getModel() << COLOR_RESET << std::endl;
            return true;
        }
        std::cout << COLOR_RED << "✗ Invalid selection. Please try again." << COLOR_RESET << std::endl;
        return false;
    }

    // Displaying available engines with improved formatting
    void displayAvailableEngines() const {
        printHeader("Available Engines");

        // Group engines by fuel type
        std::map<std::string, std::vector<std::shared_ptr<Engine>>> enginesByFuelType;

        for (const auto& engine : availableEngines) {
            enginesByFuelType[engine->getFuelType()].push_back(engine);
        }

        size_t index = 1;
        for (const auto& typePair : enginesByFuelType) {
            std::cout << COLOR_YELLOW << "\n" << typePair.first << " engines:" << COLOR_RESET << std::endl;

            for (const auto& engine : typePair.second) {
                std::cout << COLOR_CYAN << " [" << index << "] " << COLOR_RESET;
                std::cout << engine->getName() << " (" << engine->getCapacity() << "L, "
                          << engine->getHorsePower() << " HP) - " << formatPrice(engine->getPrice());

                if (engine->getCO2Emissions() > 0) {
                    std::cout << " - " << engine->getCO2Emissions() << " g/km CO2";
                }

                if (engine->getFuelConsumption() > 0) {
                    std::cout << " - " << engine->getFuelConsumption() << " l/100km";
                }

                std::cout << std::endl;
                index++;
            }
        }
    }
    // Engine selection with improved feedback
    bool selectEngine(size_t index) {
        if (currentVehicle && index >= 1 && index <= availableEngines.size()) {
            currentVehicle->setEngine(availableEngines[index - 1]);
            showLoadingAnimation("Installing engine");
            std::cout << COLOR_GREEN << "✓ Engine selected: " << availableEngines[index - 1]->getName() << COLOR_RESET << std::endl;
            return true;
        }
        std::cout << COLOR_RED << "✗ Invalid selection. Please try again." << COLOR_RESET << std::endl;
        return false;
    }

    // Displaying available equipment with improved formatting
    void displayAvailableEquipment() const {
        printHeader("Available Equipment");

        // Group equipment by category
        std::map<EquipmentCategory, std::vector<Equipment>> equipmentByCategory;

        for (const auto& equipment : availableEquipment) {
            equipmentByCategory[equipment.getCategory()].push_back(equipment);
        }

        size_t index = 1;
        for (const auto& categoryPair : equipmentByCategory) {
            std::cout << COLOR_YELLOW << "\n" << categoryToString(categoryPair.first) << ":" << COLOR_RESET << std::endl;

            for (const auto& equipment : categoryPair.second) {
                std::cout << COLOR_CYAN << " [" << index << "] " << COLOR_RESET;
                std::cout << equipment.getName() << " - " << equipment.getDescription()
                          << " - " << formatPrice(equipment.getPrice()) << std::endl;
                index++;
            }
        }
    }

    // Adding equipment with improved feedback
    bool addEquipment(size_t index) {
        if (currentVehicle && index >= 1 && index <= availableEquipment.size()) {
            currentVehicle->addEquipment(availableEquipment[index - 1]);
            return true;
        }
        std::cout << COLOR_RED << "✗ Invalid selection. Please try again." << COLOR_RESET << std::endl;
        return false;
    }

    // Displaying available colors with improved formatting
    void displayAvailableColors() const {
        printHeader("Available Colors");

        for (size_t i = 0; i < availableColors.size(); ++i) {
            std::string colorCode;
            if (availableColors[i] == "Red") colorCode = COLOR_RED;
            else if (availableColors[i] == "Blue") colorCode = COLOR_BLUE;
            else if (availableColors[i] == "Green") colorCode = COLOR_GREEN;
            else if (availableColors[i] == "Yellow") colorCode = COLOR_YELLOW;
            else if (availableColors[i] == "Black") colorCode = COLOR_BOLD;
            else colorCode = COLOR_WHITE;

            std::cout << COLOR_CYAN << " [" << i + 1 << "] " << COLOR_RESET
                      << colorCode << "■ " << availableColors[i] << COLOR_RESET << std::endl;
        }
    }
    // Color selection with improved feedback
    bool selectColor(size_t index) {
        if (currentVehicle && index >= 1 && index <= availableColors.size()) {
            currentVehicle->setColor(availableColors[index - 1]);
            showLoadingAnimation("Applying paint");
            std::cout << COLOR_GREEN << "✓ Color selected: " << availableColors[index - 1] << COLOR_RESET << std::endl;
            return true;
        }
        std::cout << COLOR_RED << "✗ Invalid selection. Please try again." << COLOR_RESET << std::endl;
        return false;
    }

    // Apply discount
    bool applyDiscount(double discountPercent) {
        if (currentVehicle && discountPercent >= 0 && discountPercent <= 30) {
            currentVehicle->setDiscount(discountPercent);
            showLoadingAnimation("Applying discount");
            std::cout << COLOR_GREEN << "✓ " << discountPercent << "% discount applied!" << COLOR_RESET << std::endl;
            return true;
        }
        std::cout << COLOR_RED << "✗ Invalid discount. Maximum allowed discount is 30%." << COLOR_RESET << std::endl;
        return false;
    }

    // Displaying current configuration
    void displayCurrentConfiguration() const {
        if (currentVehicle) {
            clearScreen();
            currentVehicle->displayInfo();
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore();
            std::cin.get();
        } else {
            std::cout << COLOR_YELLOW << "! No vehicle selected yet." << COLOR_RESET << std::endl;
        }
    }

    // Visualizing current configuration
    void visualizeCurrentConfiguration() const {
        if (currentVehicle) {
            currentVehicle->visualize();
        } else {
            std::cout << COLOR_YELLOW << "! No vehicle selected yet." << COLOR_RESET << std::endl;
        }
    }

    // Save current configuration as comparison vehicle
    void saveForComparison() {
        if (currentVehicle) {
            comparisonVehicle = currentVehicle;
            std::cout << COLOR_GREEN << "✓ Current configuration saved for comparison." << COLOR_RESET << std::endl;
        } else {
            std::cout << COLOR_YELLOW << "! No vehicle selected to save for comparison." << COLOR_RESET << std::endl;
        }
    }
    // Compare current configuration with saved one
    void compareConfigurations() const {
        if (!currentVehicle) {
            std::cout << COLOR_YELLOW << "! No current vehicle selected for comparison." << COLOR_RESET << std::endl;
            return;
        }

        if (!comparisonVehicle) {
            std::cout << COLOR_YELLOW << "! No vehicle saved for comparison." << COLOR_RESET << std::endl;
            return;
        }

        clearScreen();
        printHeader("Configuration Comparison");

        // Basic vehicle info
        std::cout << std::setw(30) << "Feature" << " | "
                  << std::setw(30) << "Current Configuration" << " | "
                  << std::setw(30) << "Saved Configuration" << std::endl;
        std::cout << std::string(95, '-') << std::endl;

        std::cout << std::setw(30) << "Vehicle" << " | "
                  << std::setw(30) << (currentVehicle->getBrand() + " " + currentVehicle->getModel()) << " | "
                  << std::setw(30) << (comparisonVehicle->getBrand() + " " + comparisonVehicle->getModel()) << std::endl;

        std::cout << std::setw(30) << "Color" << " | "
                  << std::setw(30) << currentVehicle->getColor() << " | "
                  << std::setw(30) << comparisonVehicle->getColor() << std::endl;

        std::cout << std::setw(30) << "Base Price" << " | "
                  << std::setw(30) << formatPrice(currentVehicle->getBasePrice()) << " | "
                  << std::setw(30) << formatPrice(comparisonVehicle->getBasePrice()) << std::endl;

        // Engine comparison
        std::string currentEngineName = "No engine selected";
        std::string savedEngineName = "No engine selected";

        if (currentVehicle->calculateTotalPrice() > currentVehicle->getBasePrice()) {
            currentEngineName = "Engine selected";
        }

        if (comparisonVehicle->calculateTotalPrice() > comparisonVehicle->getBasePrice()) {
            savedEngineName = "Engine selected";
        }

        std::cout << std::setw(30) << "Engine" << " | "
                  << std::setw(30) << currentEngineName << " | "
                  << std::setw(30) << savedEngineName << std::endl;

        // Total price comparison
        std::cout << std::string(95, '-') << std::endl;
        std::cout << std::setw(30) << "Total Price" << " | "
                  << std::setw(30) << formatPrice(currentVehicle->calculateTotalPrice()) << " | "
                  << std::setw(30) << formatPrice(comparisonVehicle->calculateTotalPrice()) << std::endl;

        // Price difference
        double priceDifference = currentVehicle->calculateTotalPrice() - comparisonVehicle->calculateTotalPrice();
        std::string differenceText = (priceDifference >= 0 ? "+" : "") + formatPrice(priceDifference);

        std::cout << std::setw(30) << "Price Difference" << " | "
                  << std::setw(30) << differenceText << " | "
                  << std::setw(30) << "" << std::endl;

        std::cout << "\nPress Enter to continue...";
        std::cin.ignore();
        std::cin.get();
    }
    // Saving configuration to file
    void saveConfiguration(const std::string& filename) const {
        if (currentVehicle) {
            // Create configs directory if it doesn't exist
            std::filesystem::path dirPath = "configs";
            if (!std::filesystem::exists(dirPath)) {
                std::filesystem::create_directory(dirPath);
            }

            std::string fullPath = filename;
            if (fullPath.find("configs/") == std::string::npos) {
                fullPath = "configs/" + fullPath;
            }

            if (fullPath.find(".txt") == std::string::npos) {
                fullPath += ".txt";
            }

            currentVehicle->saveToFile(fullPath);
        } else {
            std::cout << COLOR_YELLOW << "! No vehicle selected yet." << COLOR_RESET << std::endl;
        }
    }

    // Loading configuration from file with better error handling
    bool loadConfiguration(const std::string& filename) {
        std::string fullPath = filename;
        if (fullPath.find("configs/") == std::string::npos) {
            fullPath = "configs/" + fullPath;
        }

        if (fullPath.find(".txt") == std::string::npos) {
            fullPath += ".txt";
        }

        std::ifstream file(fullPath);
        if (!file.is_open()) {
            std::cerr << COLOR_RED << "✗ Cannot open file: " << fullPath << COLOR_RESET << std::endl;
            return false;
        }

        showLoadingAnimation("Loading configuration from " + fullPath);

        std::string line, version;

        // Read header
        std::getline(file, line); // VEHICLE_CONFIGURATION
        std::getline(file, version); // VERSION x.x
        std::getline(file, line); // DATE
        std::getline(file, line); // Empty line

        // Read vehicle section
        std::getline(file, line); // [VEHICLE]

        std::string brand, model, year, color;
        double basePrice, discount;

        std::getline(file, line); // BRAND=xxx
        brand = line.substr(line.find('=') + 1);

        std::getline(file, line); // MODEL=xxx
        model = line.substr(line.find('=') + 1);

        std::getline(file, line); // YEAR=xxx
        year = line.substr(line.find('=') + 1);

        std::getline(file, line); // BASE_PRICE=xxx
        basePrice = std::stod(line.substr(line.find('=') + 1));

        std::getline(file, line); // COLOR=xxx
        color = line.substr(line.find('=') + 1);

        std::getline(file, line); // DISCOUNT=xxx
        discount = std::stod(line.substr(line.find('=') + 1));

        // Find matching vehicle
        bool foundVehicle = false;
        for (size_t i = 0; i < availableVehicles.size(); ++i) {
            if (availableVehicles[i]->getBrand() == brand &&
                availableVehicles[i]->getModel() == model) {
                currentVehicle = availableVehicles[i];
                currentVehicle->setColor(color);
                currentVehicle->setDiscount(discount);
                foundVehicle = true;
                break;
            }
        }

        if (!foundVehicle) {
            std::cerr << COLOR_RED << "✗ No matching vehicle found in available vehicles." << COLOR_RESET << std::endl;
            file.close();
            return false;
        }
        // Skip empty line
        std::getline(file, line);

        // Read engine section
        std::getline(file, line); // [ENGINE]

        std::string engineName, fuelType;
        double capacity, enginePrice, fuelConsumption;
        int horsePower, co2Emissions;

        std::getline(file, line); // NAME=xxx
        engineName = line.substr(line.find('=') + 1);

        std::getline(file, line); // CAPACITY=xxx
        capacity = std::stod(line.substr(line.find('=') + 1));

        std::getline(file, line); // HORSEPOWER=xxx
        horsePower = std::stoi(line.substr(line.find('=') + 1));

        std::getline(file, line); // FUEL_TYPE=xxx
        fuelType = line.substr(line.find('=') + 1);

        std::getline(file, line); // PRICE=xxx
        enginePrice = std::stod(line.substr(line.find('=') + 1));

        std::getline(file, line); // CO2_EMISSIONS=xxx
        co2Emissions = std::stoi(line.substr(line.find('=') + 1));

        std::getline(file, line); // FUEL_CONSUMPTION=xxx
        fuelConsumption = std::stod(line.substr(line.find('=') + 1));

        // Find matching engine
        bool foundEngine = false;
        for (size_t i = 0; i < availableEngines.size(); ++i) {
            if (availableEngines[i]->getName() == engineName) {
                currentVehicle->setEngine(availableEngines[i]);
                foundEngine = true;
                break;
            }
        }

        if (!foundEngine) {
            std::cout << COLOR_YELLOW << "! No matching engine found. Engine will not be configured." << COLOR_RESET << std::endl;
        }

        // Skip empty line
        std::getline(file, line);

        // Read equipment section
        std::getline(file, line); // [EQUIPMENT]
        std::getline(file, line); // COUNT=xxx
        int equipmentCount = std::stoi(line.substr(line.find('=') + 1));

        // Skip empty line
        std::getline(file, line);

        // Read each equipment item
        for (int i = 0; i < equipmentCount; ++i) {
            std::getline(file, line); // [EQUIPMENT_ITEM_x]

            std::string equipmentName, equipmentDescription;
            double equipmentPrice;
            int categoryInt;

            std::getline(file, line); // NAME=xxx
            equipmentName = line.substr(line.find('=') + 1);

            std::getline(file, line); // DESCRIPTION=xxx
            equipmentDescription = line.substr(line.find('=') + 1);

            std::getline(file, line); // PRICE=xxx
            equipmentPrice = std::stod(line.substr(line.find('=') + 1));

            std::getline(file, line); // CATEGORY=xxx
            categoryInt = std::stoi(line.substr(line.find('=') + 1));

            // Find matching equipment
            bool foundEquipment = false;
            for (size_t j = 0; j < availableEquipment.size(); ++j) {
                if (availableEquipment[j].getName() == equipmentName) {
                    currentVehicle->addEquipment(availableEquipment[j]);
                    foundEquipment = true;
                    break;
                }
            }

            if (!foundEquipment) {
                std::cout << COLOR_YELLOW << "! No matching equipment found: " << equipmentName << COLOR_RESET << std::endl;
            }

            // Skip empty line
            if (i < equipmentCount - 1) {
                std::getline(file, line);
            }
        }

        file.close();
        std::cout << COLOR_GREEN << "✓ Configuration has been loaded from file: " << fullPath << COLOR_RESET << std::endl;
        return true;
    }

    // Checking if vehicle has been selected
    bool hasSelectedVehicle() const {
        return currentVehicle != nullptr;
    }

    // Generate PDF report of the configuration (simulated)
    void generateReport() const {
        if (!currentVehicle) {
            std::cout << COLOR_YELLOW << "! No vehicle selected yet." << COLOR_RESET << std::endl;
            return;
        }

        std::string filename = currentVehicle->getBrand() + "_" + currentVehicle->getModel() + "_report.pdf";
        showLoadingAnimation("Generating PDF report", 2);
        std::cout << COLOR_GREEN << "✓ Report has been generated: " << filename << COLOR_RESET << std::endl;
        std::cout << "  (This is a simulation - no actual PDF was created)" << std::endl;
    }
    // Show equipment by category
    void displayEquipmentByCategory() const {
        if (!currentVehicle) {
            std::cout << COLOR_YELLOW << "! No vehicle selected yet." << COLOR_RESET << std::endl;
            return;
        }

        printHeader("Current Equipment by Category");

        // Create a map to store equipment by category
        std::map<EquipmentCategory, std::vector<Equipment>> equipmentByCategory;
        double totalEquipmentCost = 0.0;

        for (const auto& equipment : currentVehicle->getSelectedEquipment()) {
            equipmentByCategory[equipment.getCategory()].push_back(equipment);
            totalEquipmentCost += equipment.getPrice();
        }

        if (equipmentByCategory.empty()) {
            std::cout << COLOR_YELLOW << "No equipment added yet." << COLOR_RESET << std::endl;
            return;
        }

        for (const auto& categoryPair : equipmentByCategory) {
            std::cout << COLOR_YELLOW << "\n" << categoryToString(categoryPair.first) << ":" << COLOR_RESET << std::endl;

            double categoryTotal = 0.0;
            for (const auto& equipment : categoryPair.second) {
                std::cout << "  ├─ " << equipment.getName() << ": " << formatPrice(equipment.getPrice()) << std::endl;
                std::cout << "  │  " << equipment.getDescription() << std::endl;
                categoryTotal += equipment.getPrice();
            }

            std::cout << "  └─ " << COLOR_BOLD << "Category total: " << formatPrice(categoryTotal)
                      << " (" << std::fixed << std::setprecision(1)
                      << (categoryTotal / totalEquipmentCost * 100) << "% of equipment cost)"
                      << COLOR_RESET << std::endl;
        }

        std::cout << COLOR_BOLD << COLOR_GREEN << "\nTotal equipment cost: "
                  << formatPrice(totalEquipmentCost) << COLOR_RESET << std::endl;
    }

    // Show available equipment by category
    void displayAvailableEquipmentByCategory() const {
        printHeader("Available Equipment by Category");

        // Create a map to store equipment by category
        std::map<EquipmentCategory, std::vector<std::pair<size_t, Equipment>>> equipmentByCategory;

        for (size_t i = 0; i < availableEquipment.size(); ++i) {
            equipmentByCategory[availableEquipment[i].getCategory()].push_back({i + 1, availableEquipment[i]});
        }

        for (const auto& categoryPair : equipmentByCategory) {
            std::cout << COLOR_YELLOW << "\n" << categoryToString(categoryPair.first) << ":" << COLOR_RESET << std::endl;

            for (const auto& indexedEquipment : categoryPair.second) {
                std::cout << COLOR_CYAN << " [" << indexedEquipment.first << "] " << COLOR_RESET;
                std::cout << indexedEquipment.second.getName() << " - "
                          << indexedEquipment.second.getDescription() << " - "
                          << formatPrice(indexedEquipment.second.getPrice()) << std::endl;
            }
        }
    }

    // Remove equipment from current vehicle
    void removeEquipmentMenu() {
        if (!currentVehicle) {
            std::cout << COLOR_YELLOW << "! No vehicle selected yet." << COLOR_RESET << std::endl;
            return;
        }

        const auto& selectedEquipment = currentVehicle->getSelectedEquipment();

        if (selectedEquipment.empty()) {
            std::cout << COLOR_YELLOW << "! No equipment to remove." << COLOR_RESET << std::endl;
            return;
        }

        printHeader("Remove Equipment");

        for (size_t i = 0; i < selectedEquipment.size(); ++i) {
            std::cout << COLOR_CYAN << " [" << i + 1 << "] " << COLOR_RESET;
            std::cout << selectedEquipment[i].getName() << " - "
                      << formatPrice(selectedEquipment[i].getPrice()) << std::endl;
        }

        std::cout << COLOR_CYAN << " [0] " << COLOR_RESET << "Cancel" << std::endl;

        size_t choice;
        std::cout << "\nSelect equipment to remove: ";
        std::cin >> choice;

        if (choice == 0) {
            return;
        }

        if (choice >= 1 && choice <= selectedEquipment.size()) {
            currentVehicle->removeEquipment(selectedEquipment[choice - 1].getName());
        } else {
            std::cout << COLOR_RED << "✗ Invalid selection." << COLOR_RESET << std::endl;
        }
    }
};
// Main user interface function with enhanced UI
void runUserInterface() {
    VehicleConfigurator configurator;
    bool running = true;

    clearScreen();
    std::cout << COLOR_BOLD << COLOR_BLUE;
    std::cout << R"(
 __      __   _     _      _        _____             __ _                       _
 \ \    / /__| |__ (_)__ _| |___   / / __| ___  _ _  / _(_)__ _ _  _ _ _ __ _ _| |_ ___ _ _ _
  \ \/\/ / -_) '_ \| / _` | / -_) / / (__/ _ \| ' \| |_| / _` | || | '_/ _` |_  _/ _ \ '_| '_|
   \_/\_/\___|_.__// \__,_|_\___| /_/\___\___/|_||_|_| |_\__, |\_,_|_| \__,_| \__\___/_| |_|
                 |__/                                     |___/
)" << COLOR_RESET << std::endl;

    std::cout << COLOR_YELLOW << "Welcome to the enhanced Vehicle Configurator!" << COLOR_RESET << std::endl;
    std::cout << "This application allows you to configure your dream vehicle with various options.\n" << std::endl;

    while (running) {
        printHeader("Main Menu");

        printMenuItem(1, "Select vehicle");
        printMenuItem(2, "Select engine");
        printMenuItem(3, "Add equipment");
        printMenuItem(4, "Remove equipment");
        printMenuItem(5, "Select color");
        printMenuItem(6, "Apply discount");
        printMenuItem(7, "Display current configuration");
        printMenuItem(8, "Visualize vehicle");
        printMenuItem(9, "Save configuration");
        printMenuItem(10, "Load configuration");
        printMenuItem(11, "Save for comparison");
        printMenuItem(12, "Compare configurations");
        printMenuItem(13, "Generate PDF report");
        printMenuItem(0, "Exit");

        int choice;
        std::cout << "\n" << COLOR_BOLD << "Your choice: " << COLOR_RESET;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                clearScreen();
                configurator.displayAvailableVehicles();
                std::cout << "\nSelect vehicle number (0 to cancel): ";
                size_t vehicleIndex;
                std::cin >> vehicleIndex;

                if (vehicleIndex == 0) break;

                if (!configurator.selectVehicle(vehicleIndex)) {
                    std::cout << "Press Enter to continue...";
                    std::cin.ignore();
                    std::cin.get();
                }
                break;
            }
            case 2: {
                if (!configurator.hasSelectedVehicle()) {
                    std::cout << COLOR_YELLOW << "! Please select a vehicle first." << COLOR_RESET << std::endl;
                    std::cout << "Press Enter to continue...";
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }

                clearScreen();
                configurator.displayAvailableEngines();
                std::cout << "\nSelect engine number (0 to cancel): ";
                size_t engineIndex;
                std::cin >> engineIndex;

                if (engineIndex == 0) break;

                if (!configurator.selectEngine(engineIndex)) {
                    std::cout << "Press Enter to continue...";
                    std::cin.ignore();
                    std::cin.get();
                }
                break;
            }
            case 3: {
                if (!configurator.hasSelectedVehicle()) {
                    std::cout << COLOR_YELLOW << "! Please select a vehicle first." << COLOR_RESET << std::endl;
                    std::cout << "Press Enter to continue...";
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }

                clearScreen();
                configurator.displayAvailableEquipmentByCategory();
                std::cout << "\nSelect equipment number (0 to finish): ";
                size_t equipmentIndex;
                std::cin >> equipmentIndex;

                while (equipmentIndex != 0) {
                    if (!configurator.addEquipment(equipmentIndex)) {
                        std::cout << "Invalid equipment selection." << std::endl;
                    }
                    std::cout << "Select next equipment (0 to finish): ";
                    std::cin >> equipmentIndex;
                }
                break;
            }
            case 4: {
                clearScreen();
                configurator.removeEquipmentMenu();
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            }
            case 5: {
                if (!configurator.hasSelectedVehicle()) {
                    std::cout << COLOR_YELLOW << "! Please select a vehicle first." << COLOR_RESET << std::endl;
                    std::cout << "Press Enter to continue...";
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }

                clearScreen();
                configurator.displayAvailableColors();
                std::cout << "\nSelect color number (0 to cancel): ";
                size_t colorIndex;
                std::cin >> colorIndex;

                if (colorIndex == 0) break;

                if (!configurator.selectColor(colorIndex)) {
                    std::cout << "Press Enter to continue...";
                    std::cin.ignore();
                    std::cin.get();
                }
                break;
            }
            case 6: {
                if (!configurator.hasSelectedVehicle()) {
                    std::cout << COLOR_YELLOW << "! Please select a vehicle first." << COLOR_RESET << std::endl;
                    std::cout << "Press Enter to continue...";
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }

                clearScreen();
                printHeader("Apply Discount");
                std::cout << "Enter discount percentage (0-30): ";
                double discountPercent;
                std::cin >> discountPercent;

                if (!configurator.applyDiscount(discountPercent)) {
                    std::cout << "Press Enter to continue...";
                    std::cin.ignore();
                    std::cin.get();
                }
                break;
            }
            case 7: {
                configurator.displayCurrentConfiguration();
                break;
            }
            case 8: {
                configurator.visualizeCurrentConfiguration();
                break;
            }
            case 9: {
                if (!configurator.hasSelectedVehicle()) {
                    std::cout << COLOR_YELLOW << "! Please select a vehicle first." << COLOR_RESET << std::endl;
                    std::cout << "Press Enter to continue...";
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }

                clearScreen();
                printHeader("Save Configuration");
                std::cout << "Enter filename (without extension): ";
                std::string filename;
                std::cin.ignore();
                std::getline(std::cin, filename);

                configurator.saveConfiguration(filename);
                std::cout << "Press Enter to continue...";
                std::cin.get();
                break;
            }
            case 10: {
                clearScreen();
                printHeader("Load Configuration");
                std::cout << "Enter filename (without extension): ";
                std::string filename;
                std::cin.ignore();
                std::getline(std::cin, filename);

                if (!configurator.loadConfiguration(filename)) {
                    std::cout << "Press Enter to continue...";
                    std::cin.get();
                }
                break;
            }
            case 11: {
                configurator.saveForComparison();
                std::cout << "Press Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            }
            case 12: {
                configurator.compareConfigurations();
                break;
            }
            case 13: {
                configurator.generateReport();
                std::cout << "Press Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            }
            case 0: {
                std::cout << COLOR_YELLOW << "Thank you for using Vehicle Configurator!" << COLOR_RESET << std::endl;
                running = false;
                break;
            }
            default: {
                std::cout << COLOR_RED << "✗ Invalid option. Please try again." << COLOR_RESET << std::endl;
                std::cout << "Press Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            }
        }
    }
}

// Main function
int main() {
    runUserInterface();
    return 0;
}

