package com.gmail.doloiu22.service;

import com.gmail.doloiu22.model.ParkingSpaceEntity;
import com.gmail.doloiu22.repository.ParkingSpaceRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.Optional;

@Service
public class ParkingSpaceService {

    @Autowired
    private ParkingSpaceRepository parkingSpaceRepository;

    Optional<ParkingSpaceEntity> findById(Long id) {
        return parkingSpaceRepository.findById(id);
    }

}
