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

/*        UserEntity user1 = new UserEntity();
        user1.setLicensePlate("delete");
        user1.setPassword("pass");

        userService.save(user1);*/

/*        UserEntity user2 = new UserEntity();
        user2.setLicensePlate("suspend");
        user2.setPassword("pass");

        userService.save(user2);*/

  /*      UserEntity user3 = new UserEntity();
        user3.setLicensePlate("change");
        user3.setPassword("pass");*/

/*        userService.save(user3);*/

        if(adminExists)
            return;

        UserEntity admin = new UserEntity();
        admin.setLicensePlate(adminName);
        admin.setPassword("pass");

        userService.save(admin);

    }
}
