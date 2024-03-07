package com.gmail.doloiu22.repository;

import com.gmail.doloiu22.model.ReportEntity;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;
import java.util.Optional;

public interface ReportRepository extends JpaRepository<ReportEntity, Long> {
    Optional<ReportEntity> findById(Long id);
    List<ReportEntity> findAll();
}
