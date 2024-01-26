package com.gmail.doloiu22.repository;

import com.gmail.doloiu22.model.SessionEntity;
import com.gmail.doloiu22.model.UserEntity;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import java.util.List;
import java.util.Optional;

@Repository
public interface SessionRepository extends JpaRepository<SessionEntity, Long> {
    Optional<SessionEntity> findBySecretID(String secretID);
    List<SessionEntity> findAllByLicensePlate(String licensePlate);
}
