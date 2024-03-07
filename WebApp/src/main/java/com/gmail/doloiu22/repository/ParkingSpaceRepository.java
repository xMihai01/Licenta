package com.gmail.doloiu22.repository;

import com.gmail.doloiu22.model.ParkingSpaceEntity;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import java.util.Optional;

@Repository
public interface ParkingSpaceRepository extends JpaRepository<ParkingSpaceEntity, Long> {

    Optional<ParkingSpaceEntity> findById(Long id);

}
