package com.gmail.doloiu22.repository;

import com.gmail.doloiu22.model.ParkingSessionEntity;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import java.util.List;
import java.util.Optional;

@Repository
public interface ParkingSessionRepository extends JpaRepository<ParkingSessionEntity, Long> {

    List<ParkingSessionEntity> findAllBySessionID(Long sessionID);
    Optional<ParkingSessionEntity> findById(Long id);

}
