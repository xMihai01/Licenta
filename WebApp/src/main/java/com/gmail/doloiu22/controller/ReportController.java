package com.gmail.doloiu22.controller;

import com.gmail.doloiu22.config.util.Role;
import com.gmail.doloiu22.model.ReportEntity;
import com.gmail.doloiu22.model.SessionEntity;
import com.gmail.doloiu22.repository.ReportRepository;
import com.gmail.doloiu22.service.SessionService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.authority.SimpleGrantedAuthority;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Objects;
import java.util.Optional;

@Controller
@RequestMapping("/report")
public class ReportController {
    @Autowired
    private SessionService sessionService;
    @Autowired
    private ReportRepository reportRepository;

    @GetMapping
    @RequestMapping("/create/{sessionID}")
    public String openCreateReport(@PathVariable int sessionID, Model model, Authentication authentication) {

        Optional<SessionEntity> session = sessionService.findById((long) sessionID);

        if (session.isEmpty() || !session.get().getLicensePlate().equals(authentication.getName()))
            return "other_errors/missing_permission";

        model.addAttribute("sessionID", sessionID);
        model.addAttribute("session", sessionService.findById((long) sessionID).get());

        return "report/create";
    }

    @GetMapping
    @RequestMapping("/list")
    public String openReportList(Model model, Authentication authentication) {

        List<ReportEntity> reports = reportRepository.findAll();

        model.addAttribute("reports", reports);
        model.addAttribute("isAdmin", authentication.getAuthorities().contains(new SimpleGrantedAuthority(Role.ADMIN.name())));

        return "report/list";
    }

    @GetMapping("/view/{reportID}")
    public String openReportView(@PathVariable Long reportID, Model model, Authentication authentication) {

        Optional<ReportEntity> report = reportRepository.findById(reportID);

        if (report.isEmpty())
            return "other_errors/report_view_not_found";

        model.addAttribute("report", report.get());

        return "report/view";
    }

    @PostMapping("/viewReport")
    public String viewReport(@RequestParam("reportID") Long reportID) {
        return "redirect:/report/view/" + reportID;
    }

    @PostMapping("/actionReport")
    public String actionReport(@RequestParam("reportID") Long reportID) {
        ReportEntity report = reportRepository.findById(reportID).get();
        report.setOpened(!report.isOpened());
        reportRepository.save(report);
        return "redirect:/report/list";
    }

    @PostMapping("/createReport")
    public String createReport(@RequestParam("details") String details, @RequestParam("timePeriod") String timePeriod, @RequestParam("contact") String contact
                                , @RequestParam("yourLicensePlate") String yourPlate, @RequestParam("suspectLicensePlate") String suspectPlate
                                , @RequestParam("sessionID") Long sessionID, Model model) {

        if (details.equals("") || contact.equals("") )
            return "other_errors/empty_report_form";

        ReportEntity report = new ReportEntity();
        report.setContact(contact);
        report.setDetails(details);
        report.setReporterLicensePlate(yourPlate);
        report.setTimePeriod(timePeriod);
        report.setSessionID(sessionID);
        report.setOpened(true);
        report.setSuspectLicensePlate(suspectPlate);

        reportRepository.save(report);

        return "others/report_submitted";
    }

}
