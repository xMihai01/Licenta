package com.gmail.doloiu22.util;

public class Utils {
    public static String getLicensePlateFromSecretID(String secretID) {
        String licensePlate = "";
        int startingIndexForPlate = 0;
        for (int index = 0; index<secretID.length(); index++) {
            if (Character.isAlphabetic(secretID.charAt(index))) {
                startingIndexForPlate = index;
                break;
            }
        }
        if (startingIndexForPlate == 0)
            return "";

        for (int indexForPlate = startingIndexForPlate; indexForPlate < secretID.length(); indexForPlate++)
            licensePlate += secretID.charAt(indexForPlate);
        return licensePlate;
    }
}
