package com.gmail.doloiu22.repository;

import com.gmail.doloiu22.model.SuspendedUserEntity;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface SuspendedUserRepository extends JpaRepository<SuspendedUserEntity, Long> {

    List<SuspendedUserEntity> findAllByUserID(Long userID);

}
