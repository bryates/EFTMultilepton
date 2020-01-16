// You should never need to touch this.
void HistMaker::run(TTreeReader & newreader, int firstevent, int lastevent)
{
    if (firstevent>-1 || lastevent>-1) newreader.SetEntriesRange(firstevent,lastevent);
    cout << "Starting event loop..." << endl;
    int count = 0;
    event_counter = 0;
    while (newreader.Next()) {
        Long64_t entry = newreader.GetCurrentEntry();
        doOneEvent();
        count++;
        // if (event_counter > 100) break;
        if (count%5000==1) cout << "Event: " << entry << endl;
    }
}
