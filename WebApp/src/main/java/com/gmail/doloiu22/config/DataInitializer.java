package com.gmail.doloiu22.config;

import com.gmail.doloiu22.config.util.Role;
import com.gmail.doloiu22.model.UserEntity;
import com.gmail.doloiu22.service.UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.ApplicationArguments;
import org.springframework.boot.ApplicationRunner;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.stereotype.Component;

import java.util.ArrayList;

@Component
public class DataInitializer implements ApplicationRunner {
    @Autowired
    private UserService userService;
    private final String adminName = "ADMIN";
    private final String adminPass = "123";
    private final Role adminRole = Role.ADMIN;

    @Override
    public void run(ApplicationArguments args) throws Exception {
        boolean adminExists = userService.findByLicensePlate(adminName).isPresent();

        if(adminExists)
            return;

        UserEntity admin = new UserEntity();
        admin.setLicensePlate(adminName);
        admin.setPassword("pass");

        userService.save(admin);
    }
}
