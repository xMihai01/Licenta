package com.gmail.doloiu22.service;

import com.gmail.doloiu22.model.UserEntity;
import org.springframework.stereotype.Service;
import org.springframework.validation.Errors;
import org.springframework.validation.ValidationUtils;
import org.springframework.validation.Validator;

@Service
public class UserValidatorService implements Validator {
    @Override
    public boolean supports(Class<?> clazz) {
        return UserEntity.class.equals(clazz);
    }

    @Override
    public void validate(Object userEntity, Errors errors) {
        UserEntity user = (UserEntity) userEntity;
        ValidationUtils.rejectIfEmptyOrWhitespace(errors, "licensePlate", "user.isLicensePlateEmpty");
        ValidationUtils.rejectIfEmptyOrWhitespace(errors, "password", "user.isPasswordEmpty");

        /*
           at least 8 digits {8,}
           at least one number (?=.*\d)
           at least one lowercase (?=.*[a-z])
           at least one uppercase (?=.*[A-Z])
           at least one special character (?=.*[@#$%^&+=])
           No space [^\s]
        */

        // SECRET ID: 17055333880505549UN00TST
        String passwordRegexPattern = "^(?:(?=.*\\d)(?=.*[a-z])(?=.*[A-Z])(?=.*[@#$%^&+=]).*)[^\s]{8,}$";

        Boolean isValidPassword = user.getPassword().matches(passwordRegexPattern);
        Boolean arePasswordsTheSame = user.getPassword().equals(user.getRepeatPassword());

        if(!isValidPassword)
            errors.rejectValue("password", "user.isValidPassword");
        if(!arePasswordsTheSame)
            errors.rejectValue("repeatPassword", "user.isPasswordTheSame");

    }
}
