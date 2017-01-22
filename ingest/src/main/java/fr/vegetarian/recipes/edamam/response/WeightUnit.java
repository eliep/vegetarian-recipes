package fr.vegetarian.recipes.edamam.response;


import java.util.Optional;

public enum WeightUnit {
    GRAM("g") {
        public double convertInGram(double value) {
            return value;
        }
    },
    MILLIGRAM("mg") {
        public double convertInGram(double value) {
            return value / 1000.0;
        }
    },
    MICROGRAM("µg") {
        public double convertInGram(double value) {
            return value / 1000000.0;
        }
    };

    public abstract double convertInGram(double value);

    private final String label;

    WeightUnit(String label) {
        this.label = label;
    }

    public static Optional<WeightUnit> buildFromLabel(String label) {
        if (label == null)
            return Optional.empty();
        for (WeightUnit unit : WeightUnit.values())
            if (unit.label.equals(label))
                return Optional.of(unit);

        return Optional.empty();
    }
}
