package com.gmail.doloiu22.service;

import com.gmail.doloiu22.model.SessionEntity;
import com.gmail.doloiu22.model.UserEntity;
import com.gmail.doloiu22.repository.UserRepository;
import lombok.RequiredArgsConstructor;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.authority.SimpleGrantedAuthority;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.security.core.userdetails.User;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;
import java.util.Optional;

@Service
@Configuration
/* Will create the necessary constructor for the DI mechanism, but I do not recommend.
 *  You should explicitly put the @Autowired annotation on the constructor. */
@RequiredArgsConstructor
public class UserService implements UserDetailsService {
    private final UserRepository userRepository;
    private final SessionService sessionService;

    @Override
    public UserDetails loadUserByUsername(String licensePlate) throws UsernameNotFoundException {
        Optional<UserEntity> optUser = userRepository.findByLicensePlate(licensePlate);
        if(optUser.isPresent()){
            UserEntity appUser = optUser.get();
            if (appUser.getLicensePlate().equals("ADMIN")) {
                return new User(appUser.getLicensePlate(), appUser.getPassword(), true, true, true
                        , true, new ArrayList<>(List.of(new SimpleGrantedAuthority("ADMIN"),new SimpleGrantedAuthority("AUTH"))));
            } else return new User(appUser.getLicensePlate(), appUser.getPassword(), true, true, true
                    , true, new ArrayList<>(List.of(new SimpleGrantedAuthority("AUTH")))
            );
        }
        throw new UsernameNotFoundException(licensePlate);
    }

    public boolean register(UserEntity user, String secretID) {
        Optional<SessionEntity> sessionEntity = sessionService.findBySecretID(secretID);
        Optional<UserEntity> userEntity = userRepository.findByLicensePlate(user.getLicensePlate());
        if (userEntity.isPresent())
            return false;
        if (sessionEntity.isEmpty())
            return false;
        if (!sessionService.isSecretIDValid(sessionEntity.get()))
            return false;
        /* Encrypt password */
        user.setPassword(new BCryptPasswordEncoder().encode(user.getPassword()));

        userRepository.save(user);
        return true;
    }

    public void save(UserEntity user) {
        /* Encrypt password */
        user.setPassword(new BCryptPasswordEncoder().encode(user.getPassword()));
        userRepository.save(user);
    }
    public void login(UserEntity user, Authentication authentication) {
        UserDetails userDetails = this.loadUserByUsername(user.getLicensePlate());
        if(Objects.isNull(userDetails))
            return;

        authentication = new UsernamePasswordAuthenticationToken(userDetails, user.getPassword(), userDetails.getAuthorities());
        // UsernamePasswordAuthenticationToken usernamePasswordAuthenticationToken = new UsernamePasswordAuthenticationToken(userDetails, user.getPassword(), userDetails.getAuthorities());
        SecurityContextHolder.getContext().setAuthentication(authentication);
    }

    public Optional<UserEntity> findByLicensePlate(String licensePlate) {
        return userRepository.findByLicensePlate(licensePlate);
    }

    public List<UserEntity> findAll() {
        return userRepository.findAll();
    }

    public UserEntity findById(Long userId) {
        return userRepository.findById(userId).get();
    }
}
