#include "utilities.h"

void makeEffPlot(char *infname = "output-0.root")
{
   TFile *inf = new TFile(infname);

   TCanvas *cClosureMergedGeneral2Pt = new TCanvas("cClosureMergedGeneral2Pt","Pt",600,600);   
   TH1F *hClosurePt_MergedGeneral2 = (TH1F*) inf->Get("hClosurePt_MergedGeneral2");
   cClosureMergedGeneral2Pt->SetLogx();
   hClosurePt_MergedGeneral2->SetAxisRange(0.5,1.5,"Y");
   TLine *lPt_MergedGeneral2 = new TLine(hClosurePt_MergedGeneral2->GetBinLowEdge(1),1,hClosurePt_MergedGeneral2->GetBinLowEdge(hClosurePt_MergedGeneral2->GetNbinsX()+1),1);
   hClosurePt_MergedGeneral2->Draw();
   lPt_MergedGeneral2->Draw();

   cClosureMergedGeneral2Pt->SaveAs("fig/ClosureMergedGeneral2_Pt.gif");
   cClosureMergedGeneral2Pt->SaveAs("fig/ClosureMergedGeneral2_Pt.C");
   cClosureMergedGeneral2Pt->SaveAs("fig/ClosureMergedGeneral2_Pt.pdf");

   TCanvas *cClosureMergedGeneral2Eta = new TCanvas("cClosureMergedGeneral2Eta","Eta",600,600);   
   TH1F *hClosureEta_MergedGeneral2 = (TH1F*) inf->Get("hClosureEta_MergedGeneral2");
   hClosureEta_MergedGeneral2->SetAxisRange(0.5,1.5,"Y");
   TLine *lEta_MergedGeneral2 = new TLine(hClosureEta_MergedGeneral2->GetBinLowEdge(1),1,hClosureEta_MergedGeneral2->GetBinLowEdge(hClosureEta_MergedGeneral2->GetNbinsX()+1),1);
   hClosureEta_MergedGeneral2->Draw();
   lEta_MergedGeneral2->Draw();
   cClosureMergedGeneral2Eta->SaveAs("fig/ClosureMergedGeneral2_Eta.gif");
   cClosureMergedGeneral2Eta->SaveAs("fig/ClosureMergedGeneral2_Eta.C");
   cClosureMergedGeneral2Eta->SaveAs("fig/ClosureMergedGeneral2_Eta.pdf");

   TCanvas *cClosureMergedGeneral2Phi = new TCanvas("cClosureMergedGeneral2Phi","Phi",600,600);   
   TH1F *hClosurePhi_MergedGeneral2 = (TH1F*) inf->Get("hClosurePhi_MergedGeneral2");
   hClosurePhi_MergedGeneral2->SetAxisRange(0.5,1.5,"Y");
   TLine *lPhi_MergedGeneral2 = new TLine(hClosurePhi_MergedGeneral2->GetBinLowEdge(1),1,hClosurePhi_MergedGeneral2->GetBinLowEdge(hClosurePhi_MergedGeneral2->GetNbinsX()+1),1);
   hClosurePhi_MergedGeneral2->Draw();
   lPhi_MergedGeneral2->Draw();
   cClosureMergedGeneral2Phi->SaveAs("fig/ClosureMergedGeneral2_Phi.gif");
   cClosureMergedGeneral2Phi->SaveAs("fig/ClosureMergedGeneral2_Phi.C");
   cClosureMergedGeneral2Phi->SaveAs("fig/ClosureMergedGeneral2_Phi.pdf");

   TCanvas *cClosureMergedGeneral2DR = new TCanvas("cClosureMergedGeneral2DR","DR",600,600);   
   TH1F *hClosureDR_MergedGeneral2 = (TH1F*) inf->Get("hClosureDR_MergedGeneral2");
   hClosureDR_MergedGeneral2->SetAxisRange(0.5,1.5,"Y");
   TLine *lDR_MergedGeneral2 = new TLine(hClosureDR_MergedGeneral2->GetBinLowEdge(1),1,hClosureDR_MergedGeneral2->GetBinLowEdge(hClosureDR_MergedGeneral2->GetNbinsX()+1),1);
   hClosureDR_MergedGeneral2->Draw();
   lDR_MergedGeneral2->Draw();
   cClosureMergedGeneral2DR->SaveAs("fig/ClosureMergedGeneral2_DR.gif");
   cClosureMergedGeneral2DR->SaveAs("fig/ClosureMergedGeneral2_DR.C");
   cClosureMergedGeneral2DR->SaveAs("fig/ClosureMergedGeneral2_DR.pdf");


   TCanvas *cMergedGeneral2Pt = new TCanvas("cMergedGeneral2Pt","Pt",600,600);   
   cMergedGeneral2Pt->SetLogy();
   cMergedGeneral2Pt->SetLogx();
   TH1F *hCorrectedPt_MergedGeneral2 = (TH1F*) inf->Get("hCorrectedPt_MergedGeneral2");
   TH1F *hRecoPt_MergedGeneral2 = (TH1F*) inf->Get("hRecoPt_MergedGeneral2");
   TH1F *hGenPt_MergedGeneral2 = (TH1F*) inf->Get("hGenPt_MergedGeneral2");
   divideBinWidth(hGenPt_MergedGeneral2);
   divideBinWidth(hRecoPt_MergedGeneral2);
   divideBinWidth(hCorrectedPt_MergedGeneral2);

   hGenPt_MergedGeneral2->SetLineColor(4);
   hGenPt_MergedGeneral2->SetMarkerColor(4);
   hGenPt_MergedGeneral2->SetFillStyle(3001);
   hGenPt_MergedGeneral2->SetFillColor(4);
   hCorrectedPt_MergedGeneral2->SetLineColor(4);
   hCorrectedPt_MergedGeneral2->SetMarkerColor(4);
   hGenPt_MergedGeneral2->Draw("hist");
   hCorrectedPt_MergedGeneral2->Draw("same");
   hRecoPt_MergedGeneral2->Draw("same");
   makeHistTitle(hGenPt_MergedGeneral2,"","(Sim)Track #phi","Arbitrary Unit");
   TLegend *legPt = myLegend(0.3,0.3,0.5,0.5);
   legPt->AddEntry(hGenPt_MergedGeneral2,"Gen p_{T}","lf");
   legPt->AddEntry(hRecoPt_MergedGeneral2,"Reco p_{T}","pl");
   legPt->AddEntry(hCorrectedPt_MergedGeneral2,"Corrected p_{T}","pl");
   legPt->Draw();

   cMergedGeneral2Pt->SaveAs("fig/MergedGeneral2_Pt.gif");
   cMergedGeneral2Pt->SaveAs("fig/MergedGeneral2_Pt.C");
   cMergedGeneral2Pt->SaveAs("fig/MergedGeneral2_Pt.pdf");

   TCanvas *cMergedGeneral2Eta = new TCanvas("cMergedGeneral2Eta","Eta",600,600);   
   TH1F *hCorrectedEta_MergedGeneral2 = (TH1F*) inf->Get("hCorrectedEta_MergedGeneral2");
   TH1F *hRecoEta_MergedGeneral2 = (TH1F*) inf->Get("hRecoEta_MergedGeneral2");
   TH1F *hGenEta_MergedGeneral2 = (TH1F*) inf->Get("hGenEta_MergedGeneral2");
   divideBinWidth(hGenEta_MergedGeneral2);
   divideBinWidth(hRecoEta_MergedGeneral2);
   divideBinWidth(hCorrectedEta_MergedGeneral2);

   hGenEta_MergedGeneral2->SetLineColor(4);
   hGenEta_MergedGeneral2->SetMarkerColor(4);
   hGenEta_MergedGeneral2->SetFillStyle(3001);
   hGenEta_MergedGeneral2->SetFillColor(4);
   hGenEta_MergedGeneral2->SetAxisRange(0,hGenEta_MergedGeneral2->GetMaximum()*1.5,"Y");
   hCorrectedEta_MergedGeneral2->SetLineColor(4);
   hCorrectedEta_MergedGeneral2->SetMarkerColor(4);
   hGenEta_MergedGeneral2->Draw("hist");
   hCorrectedEta_MergedGeneral2->Draw("same");
   hRecoEta_MergedGeneral2->Draw("same");
   makeHistTitle(hGenEta_MergedGeneral2,"","(Sim)Track #eta","Arbitrary Unit");
   TLegend *legEta = myLegend(0.3,0.3,0.5,0.5);
   legEta->AddEntry(hGenEta_MergedGeneral2,"Gen p_{T}","lf");
   legEta->AddEntry(hRecoEta_MergedGeneral2,"Reco p_{T}","pl");
   legEta->AddEntry(hCorrectedEta_MergedGeneral2,"Corrected p_{T}","pl");
   legEta->Draw();

   cMergedGeneral2Eta->SaveAs("fig/MergedGeneral2_Eta.gif");
   cMergedGeneral2Eta->SaveAs("fig/MergedGeneral2_Eta.C");
   cMergedGeneral2Eta->SaveAs("fig/MergedGeneral2_Eta.pdf");

   TCanvas *cMergedGeneral2Phi = new TCanvas("cMergedGeneral2Phi","Phi",600,600);   
   TH1F *hCorrectedPhi_MergedGeneral2 = (TH1F*) inf->Get("hCorrectedPhi_MergedGeneral2");
   TH1F *hRecoPhi_MergedGeneral2 = (TH1F*) inf->Get("hRecoPhi_MergedGeneral2");
   TH1F *hGenPhi_MergedGeneral2 = (TH1F*) inf->Get("hGenPhi_MergedGeneral2");
   divideBinWidth(hGenPhi_MergedGeneral2);
   divideBinWidth(hRecoPhi_MergedGeneral2);
   divideBinWidth(hCorrectedPhi_MergedGeneral2);

   hGenPhi_MergedGeneral2->SetLineColor(4);
   hGenPhi_MergedGeneral2->SetMarkerColor(4);
   hGenPhi_MergedGeneral2->SetFillStyle(3001);
   hGenPhi_MergedGeneral2->SetFillColor(4);
   hCorrectedPhi_MergedGeneral2->SetLineColor(4);
   hCorrectedPhi_MergedGeneral2->SetMarkerColor(4);
   hGenPhi_MergedGeneral2->Draw("hist");
   hCorrectedPhi_MergedGeneral2->Draw("same");
   hRecoPhi_MergedGeneral2->Draw("same");
   hGenPhi_MergedGeneral2->SetAxisRange(0,hGenPhi_MergedGeneral2->GetMaximum()*1.5,"Y");
   makeHistTitle(hGenPhi_MergedGeneral2,"","(Sim)Track p_{T} (GeV/c)","Arbitrary Unit");
   TLegend *legPhi = myLegend(0.2,0.7,0.4,0.9);
   legPhi->AddEntry(hGenPhi_MergedGeneral2,"Gen p_{T}","lf");
   legPhi->AddEntry(hRecoPhi_MergedGeneral2,"Reco p_{T}","pl");
   legPhi->AddEntry(hCorrectedPhi_MergedGeneral2,"Corrected p_{T}","pl");
   legPhi->Draw();

   cMergedGeneral2Phi->SaveAs("fig/MergedGeneral2_Phi.gif");
   cMergedGeneral2Phi->SaveAs("fig/MergedGeneral2_Phi.C");
   cMergedGeneral2Phi->SaveAs("fig/MergedGeneral2_Phi.pdf");

   TCanvas *cMergedGeneral2DR = new TCanvas("cMergedGeneral2DR","DR",600,600);   
   TH1F *hCorrectedDR_MergedGeneral2 = (TH1F*) inf->Get("hCorrectedDR_MergedGeneral2");
   TH1F *hRecoDR_MergedGeneral2 = (TH1F*) inf->Get("hRecoDR_MergedGeneral2");
   TH1F *hGenDR_MergedGeneral2 = (TH1F*) inf->Get("hGenDR_MergedGeneral2");
   divideBinWidth(hGenDR_MergedGeneral2);
   divideBinWidth(hRecoDR_MergedGeneral2);
   divideBinWidth(hCorrectedDR_MergedGeneral2);

   hGenDR_MergedGeneral2->SetLineColor(4);
   hGenDR_MergedGeneral2->SetMarkerColor(4);
   hGenDR_MergedGeneral2->SetFillStyle(3001);
   hGenDR_MergedGeneral2->SetFillColor(4);
   hCorrectedDR_MergedGeneral2->SetLineColor(4);
   hCorrectedDR_MergedGeneral2->SetMarkerColor(4);
   hGenDR_MergedGeneral2->Draw("hist");
   hCorrectedDR_MergedGeneral2->Draw("same");
   hRecoDR_MergedGeneral2->Draw("same");
   hGenDR_MergedGeneral2->SetAxisRange(0,hGenDR_MergedGeneral2->GetMaximum()*1.5,"Y");
   makeHistTitle(hGenDR_MergedGeneral2,"","(Sim)Track #Delta R","Arbitrary Unit");
   TLegend *legDR = myLegend(0.2,0.7,0.4,0.9);
   legDR->AddEntry(hGenDR_MergedGeneral2,"Gen p_{T}","lf");
   legDR->AddEntry(hRecoDR_MergedGeneral2,"Reco p_{T}","pl");
   legDR->AddEntry(hCorrectedDR_MergedGeneral2,"Corrected p_{T}","pl");
   legDR->Draw();

   cMergedGeneral2DR->SaveAs("fig/MergedGeneral2_DR.gif");
   cMergedGeneral2DR->SaveAs("fig/MergedGeneral2_DR.C");
   cMergedGeneral2DR->SaveAs("fig/MergedGeneral2_DR.pdf");







   TCanvas *cClosureMergedGeneralPt = new TCanvas("cClosureMergedGeneralPt","Pt",600,600);   
   TH1F *hClosurePt_MergedGeneral = (TH1F*) inf->Get("hClosurePt_MergedGeneral");
   cClosureMergedGeneralPt->SetLogx();
   hClosurePt_MergedGeneral->SetAxisRange(0.5,1.5,"Y");
   TLine *lPt_MergedGeneral = new TLine(hClosurePt_MergedGeneral->GetBinLowEdge(1),1,hClosurePt_MergedGeneral->GetBinLowEdge(hClosurePt_MergedGeneral->GetNbinsX()+1),1);
   hClosurePt_MergedGeneral->Draw();
   lPt_MergedGeneral->Draw();

   cClosureMergedGeneralPt->SaveAs("fig/ClosureMergedGeneral_Pt.gif");
   cClosureMergedGeneralPt->SaveAs("fig/ClosureMergedGeneral_Pt.C");
   cClosureMergedGeneralPt->SaveAs("fig/ClosureMergedGeneral_Pt.pdf");

   TCanvas *cClosureMergedGeneralEta = new TCanvas("cClosureMergedGeneralEta","Eta",600,600);   
   TH1F *hClosureEta_MergedGeneral = (TH1F*) inf->Get("hClosureEta_MergedGeneral");
   hClosureEta_MergedGeneral->SetAxisRange(0.5,1.5,"Y");
   TLine *lEta_MergedGeneral = new TLine(hClosureEta_MergedGeneral->GetBinLowEdge(1),1,hClosureEta_MergedGeneral->GetBinLowEdge(hClosureEta_MergedGeneral->GetNbinsX()+1),1);
   hClosureEta_MergedGeneral->Draw();
   lEta_MergedGeneral->Draw();
   cClosureMergedGeneralEta->SaveAs("fig/ClosureMergedGeneral_Eta.gif");
   cClosureMergedGeneralEta->SaveAs("fig/ClosureMergedGeneral_Eta.C");
   cClosureMergedGeneralEta->SaveAs("fig/ClosureMergedGeneral_Eta.pdf");

   TCanvas *cClosureMergedGeneralPhi = new TCanvas("cClosureMergedGeneralPhi","Phi",600,600);   
   TH1F *hClosurePhi_MergedGeneral = (TH1F*) inf->Get("hClosurePhi_MergedGeneral");
   hClosurePhi_MergedGeneral->SetAxisRange(0.5,1.5,"Y");
   TLine *lPhi_MergedGeneral = new TLine(hClosurePhi_MergedGeneral->GetBinLowEdge(1),1,hClosurePhi_MergedGeneral->GetBinLowEdge(hClosurePhi_MergedGeneral->GetNbinsX()+1),1);
   hClosurePhi_MergedGeneral->Draw();
   lPhi_MergedGeneral->Draw();
   cClosureMergedGeneralPhi->SaveAs("fig/ClosureMergedGeneral_Phi.gif");
   cClosureMergedGeneralPhi->SaveAs("fig/ClosureMergedGeneral_Phi.C");
   cClosureMergedGeneralPhi->SaveAs("fig/ClosureMergedGeneral_Phi.pdf");

   TCanvas *cClosureMergedGeneralDR = new TCanvas("cClosureMergedGeneralDR","DR",600,600);   
   TH1F *hClosureDR_MergedGeneral = (TH1F*) inf->Get("hClosureDR_MergedGeneral");
   hClosureDR_MergedGeneral->SetAxisRange(0.5,1.5,"Y");
   TLine *lDR_MergedGeneral = new TLine(hClosureDR_MergedGeneral->GetBinLowEdge(1),1,hClosureDR_MergedGeneral->GetBinLowEdge(hClosureDR_MergedGeneral->GetNbinsX()+1),1);
   hClosureDR_MergedGeneral->Draw();
   lDR_MergedGeneral->Draw();
   cClosureMergedGeneralDR->SaveAs("fig/ClosureMergedGeneral_DR.gif");
   cClosureMergedGeneralDR->SaveAs("fig/ClosureMergedGeneral_DR.C");
   cClosureMergedGeneralDR->SaveAs("fig/ClosureMergedGeneral_DR.pdf");

   TCanvas *cMergedGeneralPt = new TCanvas("cMergedGeneralPt","Pt",600,600);   
   cMergedGeneralPt->SetLogy();
   cMergedGeneralPt->SetLogx();
   TH1F *hCorrectedPt_MergedGeneral = (TH1F*) inf->Get("hCorrectedPt_MergedGeneral");
   TH1F *hRecoPt_MergedGeneral = (TH1F*) inf->Get("hRecoPt_MergedGeneral");
   TH1F *hGenPt_MergedGeneral = (TH1F*) inf->Get("hGenPt_MergedGeneral");
   divideBinWidth(hGenPt_MergedGeneral);
   divideBinWidth(hRecoPt_MergedGeneral);
   divideBinWidth(hCorrectedPt_MergedGeneral);

   hGenPt_MergedGeneral->SetLineColor(4);
   hGenPt_MergedGeneral->SetMarkerColor(4);
   hGenPt_MergedGeneral->SetFillStyle(3001);
   hGenPt_MergedGeneral->SetFillColor(4);
   hCorrectedPt_MergedGeneral->SetLineColor(4);
   hCorrectedPt_MergedGeneral->SetMarkerColor(4);
   hGenPt_MergedGeneral->Draw("hist");
   hCorrectedPt_MergedGeneral->Draw("same");
   hRecoPt_MergedGeneral->Draw("same");
   makeHistTitle(hGenPt_MergedGeneral,"","(Sim)Track #phi","Arbitrary Unit");
   legPt->Draw();

   cMergedGeneralPt->SaveAs("fig/MergedGeneral_Pt.gif");
   cMergedGeneralPt->SaveAs("fig/MergedGeneral_Pt.C");
   cMergedGeneralPt->SaveAs("fig/MergedGeneral_Pt.pdf");

   TCanvas *cMergedGeneralEta = new TCanvas("cMergedGeneralEta","Eta",600,600);   
   TH1F *hCorrectedEta_MergedGeneral = (TH1F*) inf->Get("hCorrectedEta_MergedGeneral");
   TH1F *hRecoEta_MergedGeneral = (TH1F*) inf->Get("hRecoEta_MergedGeneral");
   TH1F *hGenEta_MergedGeneral = (TH1F*) inf->Get("hGenEta_MergedGeneral");
   divideBinWidth(hGenEta_MergedGeneral);
   divideBinWidth(hRecoEta_MergedGeneral);
   divideBinWidth(hCorrectedEta_MergedGeneral);

   hGenEta_MergedGeneral->SetLineColor(4);
   hGenEta_MergedGeneral->SetMarkerColor(4);
   hGenEta_MergedGeneral->SetFillStyle(3001);
   hGenEta_MergedGeneral->SetFillColor(4);
   hGenEta_MergedGeneral->SetAxisRange(0,hGenEta_MergedGeneral->GetMaximum()*1.5,"Y");
   hCorrectedEta_MergedGeneral->SetLineColor(4);
   hCorrectedEta_MergedGeneral->SetMarkerColor(4);
   hGenEta_MergedGeneral->Draw("hist");
   hCorrectedEta_MergedGeneral->Draw("same");
   hRecoEta_MergedGeneral->Draw("same");
   makeHistTitle(hGenEta_MergedGeneral,"","(Sim)Track #eta","Arbitrary Unit");
   legEta->Draw();

   cMergedGeneralEta->SaveAs("fig/MergedGeneral_Eta.gif");
   cMergedGeneralEta->SaveAs("fig/MergedGeneral_Eta.C");
   cMergedGeneralEta->SaveAs("fig/MergedGeneral_Eta.pdf");

   TCanvas *cMergedGeneralPhi = new TCanvas("cMergedGeneralPhi","Phi",600,600);   
   TH1F *hCorrectedPhi_MergedGeneral = (TH1F*) inf->Get("hCorrectedPhi_MergedGeneral");
   TH1F *hRecoPhi_MergedGeneral = (TH1F*) inf->Get("hRecoPhi_MergedGeneral");
   TH1F *hGenPhi_MergedGeneral = (TH1F*) inf->Get("hGenPhi_MergedGeneral");
   divideBinWidth(hGenPhi_MergedGeneral);
   divideBinWidth(hRecoPhi_MergedGeneral);
   divideBinWidth(hCorrectedPhi_MergedGeneral);

   hGenPhi_MergedGeneral->SetLineColor(4);
   hGenPhi_MergedGeneral->SetMarkerColor(4);
   hGenPhi_MergedGeneral->SetFillStyle(3001);
   hGenPhi_MergedGeneral->SetFillColor(4);
   hCorrectedPhi_MergedGeneral->SetLineColor(4);
   hCorrectedPhi_MergedGeneral->SetMarkerColor(4);
   hGenPhi_MergedGeneral->Draw("hist");
   hCorrectedPhi_MergedGeneral->Draw("same");
   hRecoPhi_MergedGeneral->Draw("same");
   hGenPhi_MergedGeneral->SetAxisRange(0,hGenPhi_MergedGeneral->GetMaximum()*1.5,"Y");
   makeHistTitle(hGenPhi_MergedGeneral,"","(Sim)Track p_{T} (GeV/c)","Arbitrary Unit");
   legPhi->Draw();

   cMergedGeneralPhi->SaveAs("fig/MergedGeneral_Phi.gif");
   cMergedGeneralPhi->SaveAs("fig/MergedGeneral_Phi.C");
   cMergedGeneralPhi->SaveAs("fig/MergedGeneral_Phi.pdf");

   TCanvas *cMergedGeneralDR = new TCanvas("cMergedGeneralDR","DR",600,600);   
   TH1F *hCorrectedDR_MergedGeneral = (TH1F*) inf->Get("hCorrectedDR_MergedGeneral");
   TH1F *hRecoDR_MergedGeneral = (TH1F*) inf->Get("hRecoDR_MergedGeneral");
   TH1F *hGenDR_MergedGeneral = (TH1F*) inf->Get("hGenDR_MergedGeneral");
   divideBinWidth(hGenDR_MergedGeneral);
   divideBinWidth(hRecoDR_MergedGeneral);
   divideBinWidth(hCorrectedDR_MergedGeneral);

   hGenDR_MergedGeneral->SetLineColor(4);
   hGenDR_MergedGeneral->SetMarkerColor(4);
   hGenDR_MergedGeneral->SetFillStyle(3001);
   hGenDR_MergedGeneral->SetFillColor(4);
   hCorrectedDR_MergedGeneral->SetLineColor(4);
   hCorrectedDR_MergedGeneral->SetMarkerColor(4);
   hGenDR_MergedGeneral->Draw("hist");
   hCorrectedDR_MergedGeneral->Draw("same");
   hRecoDR_MergedGeneral->Draw("same");
   hGenDR_MergedGeneral->SetAxisRange(0,hGenDR_MergedGeneral->GetMaximum()*1.5,"Y");
   makeHistTitle(hGenDR_MergedGeneral,"","(Sim)Track #Delta R (GeV/c)","Arbitrary Unit");
   legDR->Draw();

   cMergedGeneralDR->SaveAs("fig/MergedGeneral_DR.gif");
   cMergedGeneralDR->SaveAs("fig/MergedGeneral_DR.C");
   cMergedGeneralDR->SaveAs("fig/MergedGeneral_DR.pdf");

}
